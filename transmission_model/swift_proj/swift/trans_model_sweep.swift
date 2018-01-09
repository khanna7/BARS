import io;
import sys;
import files;
import R;
import python;
import string;

string emews_root = getenv("EMEWS_PROJECT_ROOT");
string turbine_output = getenv("TURBINE_OUTPUT");

string model_sh_file = argv("model_sh");

string summarize_template = """
source('%s/R/summarize_functions.R')
f <- '%s'
a <- ''
if (file.exists(f)) {
  prev <- summarize_prev(f)
  inc <- summarize_inc(f)
  pop_size <- summarize_pop_size(f)
  a <- paste(paste(prev, collapse=','), paste(inc, collapse=','),
    paste(pop_size, collapse=','), sep = ',')
}
""";

string run_number_template = """
run_number = %i
param_line = '%s'
if param_line.find('run.number') != -1:
    params = param_line.split(',')
    v = next(i for i in params if i.split('=')[0] == 'run.number')
    run_number = int(v.split('=')[1])
""";



app (file out, file err) run_model (file shfile, string param_line, string instance)
{
    "bash" shfile param_line emews_root instance @stdout=out @stderr=err;
}

// call this to create any required directories
app (void o) make_dir(string dirname) {
  "mkdir" "-p" dirname;
}


// anything that need to be done prior to a model runs
// (e.g. file creation) can be done here
app (void o) run_prerequisites() {
  "ln" "-sf" ("%s/../r" % emews_root) turbine_output;
}

run_prerequisites() => {

  file model_sh = input(model_sh_file);
  file upf = input(argv("f"));
  string upf_lines[] = file_lines(upf);
  string results[];
  foreach s,i in upf_lines {
    string py_code = run_number_template % (i, s);
    string run_num = python(py_code, "str(run_number)");
    string instance = "%s/instance_%i/" % (turbine_output, run_num);
    make_dir(instance) => {
      file out <instance+"out.txt">;
      file err <instance+"err.txt">;
      (out,err) = run_model(model_sh, s, instance) => {
        string summarize_code = summarize_template % (emews_root, instance + "output/counts.csv");
        string result = R(summarize_code, "a");
        if (length(result) > 0) {
          results[i] = "%i,%s" % (run_num, result);
        } else {
          results[i] = "";
        }
      }
    }
  }

  string finals[] = pick_regexp(".+", results);

  string fname = "%s/final_results.csv" % turbine_output;
  file results_file <fname> = write(join(finals, "\n") + "\n") =>
  printf("Writing final result to %s", fname);
}
