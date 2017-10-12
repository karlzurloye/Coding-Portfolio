
def alignment_dfs(english,japanese,index,alignment):
    e_len = len(english)
    j_len = len(japanese)
    if j_len == 0:
        print alignment
        return
    if e_len <= j_len:
        alignment.append(index)
        alignment_dfs(english,japanese[1:],index,alignment)
        alignment.pop()
    if e_len > 1:
        alignment.append(index+1)
        alignment_dfs(english[1:],japanese[1:],index+1,alignment)
        alignment.pop()
    return
                      
###
                      
f1 = open("epron-jpron-unsupervised.data", "r")

line_count = 1
for line in f1:
    if (line_count + 2) % 3 == 0:
        english_str = line
        english_str = english_str.replace('"','')
        english = english_str.split()
    elif (line_count + 1) % 3 == 0:
        japanese_str = line
        japanese_str = japanese_str.replace('"','')
        japanese = japanese_str.split()
    else:
        print '\n' + str(english)
        print str(japanese)

        alignment_dfs(english,japanese[1:],1,[1])

    line_count += 1
    if line_count == 16:
        break






