
m = 54
with open('test-data-1.sent','r') as f1:
    for sentence_str in f1:
        sentence_str = sentence_str.replace('"','')
        sentence = sentence_str.split()
        print '\n' + ' '.join(sentence)
        
        Q = [[0 for x in range(m)] for y in range(len(sentence))]
        best_prev = [[0 for x in range(m)] for y in range(len(sentence))]

        word_count = 0
        for word in sentence:
            print '\n' + word + '\n'
            if word_count == 0:
                count = 0
                with open('unigram-data.txt','r') as f2:
                    for unigram_str in f2:
                        unigram = unigram_str.split()
                        tag = unigram[1]
                        p_tag = float(unigram[2])

                        no_data = True
                        with open('tag-to-word-data.txt','r') as f3:
                            for line_str in f3:
                                line = line_str.split()
                                if (line[0] == tag) & (line[1] == word):
                                    no_data = False
                                    p_word_given_tag = float(line[2])
                                    break
                        if no_data:
                            p_word_given_tag = 0

                        Q[0][count] = p_tag * p_word_given_tag
                        count += 1
                        print(str(count-1) + ' ' + str(tag) + ' ' + str(Q[0][count-1]))
                word_count += 1
            else:
                count = 0
                with open('unigram-data.txt','r') as f2:
                    for unigram_str in f2:
                        unigram = unigram_str.split()
                        tag = unigram[1]
            
                        best_score = 0
                        prev_count = 0
                        with open('unigram-data.txt','r') as f4:
                            for unigram_prev_str in f4:
                                unigram_prev = unigram_prev_str.split()
                                tag_prev = unigram_prev[1]

                                with open('bigram-data.txt','r') as f5:
                                    for bigram_str in f5:
                                        bigram = bigram_str.split()
                                        if (bigram[0] == tag_prev) & (bigram[1] == tag):
                                            p_bigram = float(bigram[2])
                                        
                                no_data = True
                                with open('tag-to-word-data.txt','r') as f3:
                                    for line_str in f3:
                                        line = line_str.split()
                                        if (line[0] == tag) & (line[1] == word):
                                            no_data = False
                                            p_word_given_tag = float(line[2])
                                            break
                                if no_data:
                                    p_word_given_tag = 0

                                r = p_bigram * p_word_given_tag * Q[word_count-1][prev_count]
                                    
                                if r > best_score:
                                    best_score = r
                                    Q[word_count][count] = r
                                    best_prev[word_count][count] = prev_count
                                
                                prev_count += 1
                        count += 1
                        print(str(count-1) + ' ' + str(tag) + ' ' + str(best_score))
                word_count += 1
              
        word_count -= 1
        final_score = 0
        for ii in xrange(0,m):
            if Q[word_count][ii] > final_score:
                final_score = Q[word_count][ii]
                final_best = ii

        final_tags = []
        count = 0
        with open('unigram-data.txt','r') as f6:
            for unigram_str in f6:
                unigram = unigram_str.split()
                if count == final_best:
                    final_tags = [unigram[1]]
                    prev = best_prev[word_count][count]
                    break
                count += 1
    
        for ii in xrange(word_count-1,-1,-1):
            count = 0
            with open('unigram-data.txt','r') as f7:
                for unigram_str in f7:
                    unigram = unigram_str.split()
                    if count == prev:
                        final_tags.insert(0,unigram[1])
                        prev = best_prev[ii][count]
                        break
                    count += 1

        print '\n' + ' '.join(sentence)
        print ' '.join(final_tags) + '\n'


