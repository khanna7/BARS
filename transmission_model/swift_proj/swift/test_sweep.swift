import io;
import sys;
import files;

app (file out, file err) trans_model (file shfile, string model_dir, string props_file, string param_line)
{
    "bash" shfile model_dir props_file param_line @stdout=out @stderr=err;
}

app (void o) make_dir(string dirname) {
  "mkdir" "-p" dirname;
}


file trans_model_sh = input("../scripts/trans_model.sh");
string model_dir = argv("md");
string props_file = argv("props");

file upf = input(argv("f"));
string upf_lines[] = file_lines(upf);

foreach s,i in upf_lines {
  string base_out = "%s/out/output" % model_dir;
  string out_dir = "%s_%i" % (base_out, i);
  make_dir(out_dir) => {
    file out <out_dir + "/out.txt">;
    file err <out_dir + "/err.txt">;
    string param_line = "run.number=%i,output.directory=%s,%s" % (i, base_out, s);
    (out,err) = trans_model(trans_model_sh, model_dir, props_file, param_line);
  }
}
