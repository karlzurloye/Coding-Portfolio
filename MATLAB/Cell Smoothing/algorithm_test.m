function [] = algorithm_test()

% this code is intended to test the functionality of the LWLR algorithm

[simple_grid] = simple_init();
safety_map(simple_grid);
title('Simple Generated Dataset');
[simple_training,simple_removed] = training(simple_grid,70);
safety_map(simple_training);
title('Simple Generated Training Dataset');
simple_repaired = lwlr(simple_training,3);
safety_map(simple_repaired);
title('Simple Generated Repaired Dataset');
err_hist(simple_repaired,simple_grid,simple_removed);