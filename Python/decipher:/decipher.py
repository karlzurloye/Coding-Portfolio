
import math

def add_log(logx, logy):
    if logx == float('-inf'): return logy
    if logy == float('-inf'): return logx
    if logx - logy > 16: return logx
    if logx > logy: return logx + math.log(1 + math.exp(logy - logx))
    if logy - logx > 16: return logy
    if logy > logx: return logy + math.log(1 + math.exp(logx - logy))
    if logx == logy: return logx + math.log(2)

# english letter bigram creation

f1 = open('english.data', 'r')

tag = ['A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',' ']
dict_bigram_count = {}
dict_bigram = {}

prev = ' '
for line in f1:
    for letter in line:
        if letter != '\n':
            bigram = letter + prev
            prev = letter
            if bigram in dict_bigram_count:
                dict_bigram_count[bigram] += 1
            else:
                dict_bigram_count[bigram] = 1

total = 0
for bigram in dict_bigram_count:
    total += dict_bigram_count[bigram]
for bigram in dict_bigram_count:
    dict_bigram[bigram] = math.log(float(dict_bigram_count[bigram]) / total)

min_prob = 0
for letter in range(0,len(tag)):
    bigram = tag[letter] + ' '
    if bigram in dict_bigram:
        if dict_bigram[bigram] < min_prob:
            min_prob = dict_bigram[bigram]
for letter in range(0,len(tag)):
    bigram = tag[letter] + ' '
    if bigram not in dict_bigram:
        dict_bigram[bigram] = min_prob

# forward backward algorithm

with open('cipher.data', 'r') as f2:
    cipher = f2.read().replace('\n',' ')

Q_a = [[0 for x in range(len(tag))] for y in range(2*len(cipher))]
Q_b = [[0 for x in range(len(tag))] for y in range(2*len(cipher))]
dict_decipher = {}
dict_decipher_count = {}

for cipher_letter in range(0,len(tag)):
    for decoded_letter in range(0,len(tag)):
        dict_decipher[tag[cipher_letter] + tag[decoded_letter]] = math.log(1.0/(len(tag)-1))
for letter in range(0,len(tag)):
    dict_decipher[tag[letter] + tag[-1]] = float('-inf')
    dict_decipher[tag[-1] + tag[letter]] = float('-inf')
dict_decipher[tag[-1] + tag[-1]] = 0

max_iteration = 50
iteration = 1
while iteration <= max_iteration:
    
    # forward pass
    for letter_column in range(0,len(Q_a)):
        for tag_node in range(0,len(Q_a[0])):
            if letter_column == 0:
                bigram = tag[tag_node] + ' '
                Q_a[letter_column][tag_node] = dict_bigram[bigram]
            elif (letter_column % 2) == 0:
                Q_a[letter_column][tag_node] = float('-inf')
                for tag_node_prev in range(0,len(Q_a[0])):
                    bigram = tag[tag_node] + tag[tag_node_prev]
                    if bigram in dict_bigram:
                        Q_a[letter_column][tag_node] = add_log(Q_a[letter_column][tag_node],Q_a[letter_column-1][tag_node_prev] + dict_bigram[bigram])
            else:
                substitution = cipher[(letter_column-1)/2] + tag[tag_node]
                Q_a[letter_column][tag_node] = Q_a[letter_column-1][tag_node] + dict_decipher[substitution]
    a_final = float('-inf')
    for tag_node in range(0,len(Q_a[0])):
        a_final = add_log(a_final,Q_a[-1][tag_node])

    # backward pass
    for letter_column in range(len(Q_b)-1,-1,-1):
        for tag_node in range(0,len(Q_b[0])):
            if letter_column == len(Q_b)-1:
                Q_b[letter_column][tag_node] = 0
            elif (letter_column % 2) == 0:
                substitution = cipher[(letter_column)/2] + tag[tag_node]
                Q_b[letter_column][tag_node] = Q_b[letter_column+1][tag_node] + dict_decipher[substitution]
            else:
                Q_b[letter_column][tag_node] = float('-inf')
                for tag_node_prev in range(0,len(Q_b[0])):
                    bigram = tag[tag_node_prev] + tag[tag_node]
                    if bigram in dict_bigram:
                        Q_b[letter_column][tag_node] = add_log(Q_b[letter_column][tag_node],Q_b[letter_column+1][tag_node_prev] + dict_bigram[bigram])

    # counting pass
    for letter_column in range(0,len(Q_a),2):
        for tag_node in range(0,len(Q_a[0])):
            substitution = cipher[letter_column/2] + tag[tag_node]
            if substitution in dict_decipher_count:
                dict_decipher_count[substitution] = add_log(dict_decipher_count[substitution],Q_a[letter_column][tag_node] + Q_b[letter_column+1][tag_node] + dict_decipher[substitution] - a_final)
            else:
                dict_decipher_count[substitution] = Q_a[letter_column][tag_node] + Q_b[letter_column+1][tag_node] + dict_decipher[substitution] - a_final
    for decoded_letter in range(0,len(tag)):
        total = float('-inf')
        for cipher_letter in range(0,len(tag)):
            substitution = tag[cipher_letter] + tag[decoded_letter]
            if substitution in dict_decipher_count:
                total = add_log(total,dict_decipher_count[substitution])
        for cipher_letter in range(0,len(tag)):
            substitution = tag[cipher_letter] + tag[decoded_letter]
            if substitution in dict_decipher_count:
                dict_decipher[substitution] = dict_decipher_count[substitution] - total
    dict_decipher[tag[-1] + tag[-1]] = 0
    for substitution in dict_decipher_count:
        dict_decipher_count[substitution] = float('-inf')

    print 'P(cipher): ' + str(a_final)

    iteration += 1

# viterbi algorithm

Q = [[0 for x in range(len(tag))] for y in range(len(cipher))]
best_prev = [[0 for x in range(len(tag))] for y in range(len(cipher))]

for tag_node in range(0,len(Q[0])):
    Q[0][tag_node] = Q_a[1][tag_node]

for letter_column in range(1,len(Q)):
    for tag_node in range(0,len(Q[0])):
        Q[letter_column][tag_node] = float('-inf')
        best_prev[letter_column][tag_node] = -1
        best_score = float('-inf')
        for tag_node_prev in range(0,len(Q[0])):
            bigram = tag[tag_node] + tag[tag_node_prev]
            if bigram in dict_bigram:
                score = Q[letter_column-1][tag_node_prev] + dict_bigram[bigram] + dict_decipher[cipher[letter_column]+tag[tag_node]]
            else:
                score = float('-inf')
            if score > best_score:
                best_score = score
                best_prev[letter_column][tag_node] = tag_node_prev
                Q[letter_column][tag_node] = score
final_best = -1
final_score = float('-inf')
for tag_node in range(0,len(Q[0])):
    if Q[-1][tag_node] > final_score:
        final_score = Q[-1][tag_node]
        final_best = tag_node

decoding = [tag[final_best]]
current = final_best
for letter_column in range(len(Q)-2,-1,-1):
    decoding.insert(0,tag[best_prev[letter_column+1][current]])
    current = best_prev[letter_column+1][current]

print '\n' + cipher
print '\n' + ''.join(decoding) + '\n'

#for substitution in dict_decipher:
#    if dict_decipher[substitution] >= math.log(0.01):
#        print 'P(' + str(substitution)[0] + '|' + str(substitution)[1] + '): ' + str(math.exp(dict_decipher[substitution]))






