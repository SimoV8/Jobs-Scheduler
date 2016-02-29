import argparse
import xlwt
import os
import re
import shutil

def writeInSheet(sheet, n, data):
    for i in range(len(data)):
        sheet.write(n, i, data[i])

def main():
    # parse inout arguments
    parser = argparse.ArgumentParser(description='Test the job scheduler with all benchmark files.')
    parser.add_argument('-e', '--exe', default='JobsScheduler.exe', type=str, help='the executable path to use for benchmark')
    parser.add_argument('-b', '--benchmarkFolder', default='./', type=str, help='the benckmark folder to search in')
    args = parser.parse_args()

    # Take a list of all file to use for benchmark
    files =  [f for f in os.listdir(args.benchmarkFolder) if re.match(r'I(_[0-9]+){5}\.csv', f)]

    best = {}
    # Read best results:
    with open("Best.csv", "r") as bestfile:
        for line in bestfile:
            v = line.split(';')
            if v[1].isdigit():
                best[v[0]] = int(v[1])


    # Inizialize xls file to store result
    book = xlwt.Workbook(encoding="utf-8")
    sheets = {}
    sheetRows = {}


    for f in files:
        print('Evaluating file '+ f)
        fileargs = list(map(int, re.findall(r'\d+', f)))
        n = fileargs[0]
        m = fileargs[1]
        b = best[f[0:-4]]
        theBest = b
        inputFile = os.path.join(args.benchmarkFolder, f)
        time = int(0.12 * n * m)
        sheet = None
        if n not in sheets:
            sheets[n] = book.add_sheet(str(n) + ' jobs')
            header = ['file name', 't1', 'v1', 't2', 'v2', 't3', 'v3', 't4', 'v4', 't5', 'v5', 'best', 'min', 'max', 'min deviation', 'max deviation']
            writeInSheet(sheets[n], 0, header)
            sheetRows[n] = 1
        sheet = sheets[n]
        row = [f]

        for i in range(5):
            os.system(args.exe + ' ' + inputFile + ' ' + str(time) + ' output/')
            solutionfile =  f[0:-4] + '_sol.csv'
            with open(os.path.join('output', solutionfile), "r") as solution:
                out = solution.readlines()
                line = out[0].split(';')
                twt = int(line[1])
                if twt <= theBest:
                    theBest = twt
                    shutil.copyfile(os.path.join('output', solutionfile), os.path.join('best', solutionfile))


        res = []
        objectives = []
        with open("output.csv", "r") as outputFile:
            for line in outputFile:
                if line.startswith(f):
                    res.append(line)
        res = res[-5:]
        for result in res:
            result = result.split(';')
            row.append(result[1])
            row.append(result[2])
            objectives.append(int(result[2]))
        minObj = min(objectives)
        maxObj = max(objectives)

        row.append(b)
        row.append(minObj)
        row.append(maxObj)
        if b != 0:
            row.append(float(minObj - b) / b)
            row.append(float(maxObj - b) / b)

        writeInSheet(sheet, sheetRows[n], row)
        sheetRows[n] = sheetRows[n] + 1;
        book.save("risultati.xls")

if __name__ == "__main__":
    main()