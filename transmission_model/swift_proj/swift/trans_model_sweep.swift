import io;
import sys;
import files;
import R;
import string;

string emews_root = getenv("EMEWS_PROJECT_ROOT");
string turbine_output = getenv("TURBINE_OUTPUT");

string summarize_template = """
source('%s/R/summarize_functions.R')
f <- %s
prev <- summarize_prev(f)
inc <- summarize_inc(f)
pop_size <- summarize_pop_size(f)
a <- paste(paste(prev, collapse=","), paste(inc, collapse=","),
  paste(pop_size, collapse=","), sep = ",")
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
  file model_sh = input(emews_root+"/scripts/trans_model.sh");
  file upf = input(argv("f"));
  string upf_lines[] = file_lines(upf);
  string results[];
  foreach s,i in upf_lines {
    string instance = "%s/instance_%i/" % (turbine_output, i+1);
    make_dir(instance) => {
      file out <instance+"out.txt">;
      file err <instance+"err.txt">;
      (out,err) = run_model(model_sh, s, instance) => {
        string summarize_code = summarize_template % (emews_root, instance + "output/counts.csv");
        string result = R(summarize_code, "a");
        results[i] = "%i,%s" % (i + 1, result);
      }
    }
  }

  string fname = "%s/final_results.csv" % turbine_output;
  file results_file <fname> = write(join(results, "\n")) =>
  printf("Writing final result to %s", fname);
}
