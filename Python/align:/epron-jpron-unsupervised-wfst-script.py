
import copy

def alignment_dfs(english,japanese,index,alignment,alignment_list):
    e_len = len(english)
    j_len = len(japanese)
    if j_len == 0:
        alignment_list.append(copy.copy(alignment))
        return alignment_list
    if e_len <= j_len:
        alignment.append(index)
        alignment_list = alignment_dfs(english,japanese[1:],index,alignment,alignment_list)
        alignment.pop(-1)
    if e_len > 1:
        alignment.append(index+1)
        alignment_list = alignment_dfs(english[1:],japanese[1:],index+1,alignment,alignment_list)
        alignment.pop(-1)
    return alignment_list

###

f = open('epron-jpron-unsupervised.wfst','w')

dict_count = {}
dict_prob = {}
max_iteration = 5
iteration = 1
while iteration <= max_iteration:
    line_count = 1
    with open('epron-jpron-unsupervised.data','r') as f1:
        for line in f1:
            if (line_count + 2) % 3 == 0:
                english_str = line
                english_str = english_str.replace('"','')
                english = english_str.split()
            elif (line_count + 1) % 3 == 0:
                japanese_str = line
                japanese_str = japanese_str.replace('"','')
                japanese = japanese_str.split()

                alignment_list = alignment_dfs(english,japanese[1:],1,[1],[])
                probability_list = [1 for x in range(len(alignment_list))]
                len_p_list = len(probability_list)
                
                if iteration == 1:
                    for ii in range(0,len_p_list):
                        probability_list[ii] = 1.0/len_p_list
                else:
                    for ii in range(0,len_p_list):
                        probability_list[ii] = 1.0
                        for jj in range(0,len(english)):
                            J = []
                            for kk in range(0,len(japanese)):
                                if alignment_list[ii][kk] == jj+1:
                                    J.append(japanese[kk])
                            J.append(english[jj])
                            J_str = str(J).replace("'",'').replace(',','')
                            probability_list[ii] *= dict_prob[J_str]
                
                    total = 0
                    for ii in range(0,len_p_list):
                        total += probability_list[ii]
                    for ii in range(0,len_p_list):
                        probability_list[ii] /= total

                for ii in range(0,len_p_list):
                    for jj in range(0,len(english)):
                        J = []
                        for kk in range(0,len(japanese)):
                            if alignment_list[ii][kk] == jj+1:
                                J.append(japanese[kk])
                        J.append(english[jj])
                        J_str = str(J).replace("'",'').replace(',','')
                        if J_str in dict_count:
                            dict_count[J_str] += probability_list[ii]
                        else:
                            dict_count[J_str] = probability_list[ii]
        
            line_count += 1

        total = 0
        for key in dict_count:
            total += dict_count[key]
        for key in dict_count:
            dict_prob[key] = dict_count[key] / total

        for key in dict_count:
            dict_count[key] = 0

        if iteration == max_iteration:
            f.write('0\n')
            index = 1
            for key in dict_prob:
                key_clean = key.replace('[','').replace(']','').replace(',','').split()
                f.write('(0 (%r "%s" "%s" %s))\n' % (index, key_clean[-1], key_clean[0], dict_prob[key]))
                index += 1
                if len(key.split()) > 2:
                    for ii in range(0,len(key_clean)-2):
                        f.write('(%r (%r *e* "%s" 1))\n' % (index-1, index, key_clean[ii+1]))
                        index += 1
                f.write('(%r (0))\n' % (index-1))

        iteration += 1





