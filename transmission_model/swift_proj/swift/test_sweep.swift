import io;
import sys;
import files;

string tproot = getenv("T_PROJECT_ROOT");

app (file out, file err) trans_model (file shfile, string m_dir, string p_file, string p_line)
{
    "bash" shfile m_dir p_file p_line @stdout=out @stderr=err;
}

app (void o) make_dir(string dirname) {
  "mkdir" "-p" dirname;
}


file trans_model_sh = input(tproot + "/scripts/trans_model.sh");
string model_dir = argv("md");
string props_file = argv("props");
string exp = input(argv("exp"));

file upf = input(argv("f"));
string upf_lines[] = file_lines(upf);

foreach s,i in upf_lines {
  string base_out = "%s/out_%s/output" % (model_dir, exp);
  string out_dir = "%s_%i" % (base_out, i);
  make_dir(out_dir) => {
    file out <out_dir + "/out.txt">;
    file err <out_dir + "/err.txt">;
    string param_line = "run=%i,output.directory=%s,%s" % (i, base_out, s);
    (out,err) = trans_model(trans_model_sh, model_dir, props_file, param_line);
  }
}
