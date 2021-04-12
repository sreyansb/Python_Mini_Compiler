import csv
import copy

PATH_TO_CSV = r"./test1.tsv"
PATH_TO_OUTPUT_1 = r"./test_output_1.tsv"

file_input = open(PATH_TO_CSV)
quads = list(csv.reader(file_input, delimiter='\t'))

def dead_code_elimination(quads):
    flag = True
    remove = True
    while(flag):
        flag = False
        for i in range(len(quads)):
            remove = True
            if(not (quads[i][4] == "-" or quads[i][1].lower() in ["label", "goto", "if false"])):
                for j in range(len(quads)):
                    if((quads[i][4] == quads[j][2] and quads[j][0] != "-1") or (quads[i][4] == quads[j][3] and quads[j][0] != "-1")):
                        remove = False
                if((remove == True) and (quads[i][0] != "-1")):
                    quads[i][0] = "-1"
                    flag = True
    return quads

quads_copy = copy.deepcopy(quads)
quads_output_1 = dead_code_elimination(quads_copy[1:])
quads_output_1.insert(0, quads[0])

file_output = open(PATH_TO_OUTPUT_1, "w")
csv_writer = csv.writer(file_output, delimiter='\t', lineterminator = "\n")
csv_writer.writerows(quads_output_1)
file_output.flush()
file_output.close()
file_input.close()
