import sys, glob, os
from rpy2 import robjects

code = """
f <- '{}'
prev <- summarize_prev(f)
inc <- summarize_inc(f)
pop_size <- summarize_pop_size(f)
"""

def main(root, start, end):
    robjects.r.source("../R/summarize_functions.R")
    expected_instances = set(range(start, end + 1))
    instances = ["{}/instance_{}".format(root, x) for x in expected_instances]
    results = []
    for d in instances:
        instance = int(d[d.rfind("_") + 1 : ])
        if os.path.exists("{}/output/main_network_50000.RDS".format(d)):
            expected_instances.remove(instance)
            robjects.r(code.format("{}/output/counts.csv".format(d)))
            prev = ",".join([str(x) for x in robjects.r['prev']])
            inc = ",".join([str(x) for x in robjects.r['inc']])
            pop_size = ",".join([str(x) for x in robjects.r['pop_size']])
            results.append("{},{},{},{}\n".format(instance, prev, inc, pop_size))

    with open("final_results_{}_{}.csv".format(start, end), 'w') as f_out:
        f_out.writelines(results)

    with open("missing_instances_{}_{}.csv".format(start, end), 'w') as f_out:
        for i in expected_instances:
            f_out.write("{}\n".format(i))


if __name__ == '__main__':
    main(sys.argv[1], int(sys.argv[2]), int(sys.argv[3]))
