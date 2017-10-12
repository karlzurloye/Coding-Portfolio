
import linecache

f1 = open('epron-jpron.alignment','r')
f2 = open('epron-jpron.data','r')

for line_count,line in enumerate(f1):
    if line_count == 0:
        correct_count = 0
        incorrect_count = 0
    elif (line_count + 2) % 3 == 0:
        data = linecache.getline('epron-jpron.alignment',line_count)
        ref = linecache.getline('epron-jpron.data',line_count)
        if data != ref:
            print 'ERROR'
    elif (line_count + 1) % 3 == 0:
        data = linecache.getline('epron-jpron.alignment',line_count)
        ref = linecache.getline('epron-jpron.data',line_count)
        if data != ref:
            print 'ERROR'
    else:
        data = linecache.getline('epron-jpron.alignment',line_count)
        ref = linecache.getline('epron-jpron.data',line_count)
        for ii in range(0,len(data)):
            if data[ii] == ref[ii]:
                correct_count += 1
            else:
                incorrect_count += 1

print str(100.0 * (correct_count - incorrect_count) / (correct_count + incorrect_count)) + '% accurate'


