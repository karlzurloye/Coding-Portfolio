void part_one_classifier(float **data_train,float **data_test)
{
    float w_x = 0;
    float w_y = 0;
    int b = 0;
    int data_class;
    int y;
    int correct = 0;
    
    while (correct < TRAINING_SIZE) {
        correct = 0;
        for (int ii=0; ii<TRAINING_SIZE; ii++) {
            if (w_x*data_train[ii][0] + w_y*data_train[ii][1] + b >= 0) {
                data_class = 1;
                y = 1;
            } else {
                data_class = 0;
                y = -1;
            }
            
            if (data_class != data_train[ii][2]) {
                w_x -= y*data_train[ii][0];
                w_y -= y*data_train[ii][1];
                b -= y;
            } else {
                correct++;
            }
        }
    }
    
    for (int ii=0; ii<TEST_SIZE; ii++) {
        if (w_x*data_test[ii][0] + w_y*data_test[ii][1] + b >= 0) {
            data_test[ii][2] = 1;
        } else {
            data_test[ii][2] = 0;
        }
    }
}

void part_two_classifier(float **data_train,float **data_test)
{
    float w[9][2];
    float b[9];
    int data_class;
    float y;
    float max_y;
    int max_class;
    int correct_class;
    int correct = 0;
    
    for (int ii=0; ii<9; ii++) {
        b[ii] = 0;
        for (int jj=0; jj<2; jj++) {
            w[ii][jj] = 0;
        }
    }
    
    while (correct < 0.95*TRAINING_SIZE) {
        correct = 0;
        for (int ii=0; ii<TRAINING_SIZE; ii++) {
            max_y = -100000;
            max_class = 0;
            for (int jj=0; jj<9; jj++) {
                y = w[jj][0]*data_train[ii][0] + w[jj][1]*data_train[ii][1] + b[jj];
                if (y > max_y) {
                    max_y = y;
                    max_class = jj;
                }
            }
            
            correct_class = data_train[ii][2];
            if (max_class != correct_class) {
                w[max_class][0] -= data_train[ii][0];
                w[max_class][1] -= data_train[ii][1];
                b[max_class]--;
                w[correct_class][0] += data_train[ii][0];
                w[correct_class][1] += data_train[ii][1];
                b[correct_class]++;
            } else {
                correct++;
            }
        }
        
        for (int ii=0; ii<TEST_SIZE; ii++) {
            max_y = -100000;
            max_class = 0;
            for (int jj=0; jj<9; jj++) {
                y = w[jj][0]*data_test[ii][0] + w[jj][1]*data_test[ii][1] + b[jj];
                if (y > max_y) {
                    max_y = y;
                    max_class = jj;
                }
            }
            data_test[ii][2] = max_class;
        }
    }
}