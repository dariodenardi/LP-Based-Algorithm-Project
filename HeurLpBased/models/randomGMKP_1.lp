\ENCODING=ISO-8859-1
\Problem name: GMKP - Callable Library

Maximize
 obj: 50 x_1_1 + 82 x_1_2 + 18 x_1_3 + 28 x_1_4 + 43 x_1_5 + 78 x_1_6
      + 72 x_1_7 + 25 x_1_8 + 97 x_1_9 + 69 x_1_10 + 61 x_1_11 + 49 x_1_12
      + 45 x_1_13 + 86 x_1_14 + 68 x_1_15 + 76 x_1_16 + 85 x_1_17 + 100 x_1_18
      + 89 x_1_19 + 35 x_1_20 + 69 x_1_21 + 79 x_1_22 + 36 x_1_23 + 10 x_1_24
      + 54 x_1_25 + 52 x_2_1 + 65 x_2_2 + 72 x_2_3 + 38 x_2_4 + 45 x_2_5
      + 73 x_2_6 + 73 x_2_7 + 41 x_2_8 + 63 x_2_9 + 42 x_2_10 + 25 x_2_11
      + 79 x_2_12 + 84 x_2_13 + 71 x_2_14 + 29 x_2_15 + 23 x_2_16 + 42 x_2_17
      + 11 x_2_18 + 10 x_2_19 + 17 x_2_20 + 18 x_2_21 + 100 x_2_22 + 97 x_2_23
      + 18 x_2_24 + 77 x_2_25
Subject To
 capacity_1:            70 x_1_1 + 90 x_1_2 + 58 x_1_3 + 72 x_1_4 + 100 x_1_5
                        + 24 x_1_6 + 90 x_1_7 + 56 x_1_8 + 15 x_1_9 + 49 x_1_10
                        + 60 x_1_11 + 98 x_1_12 + 81 x_1_13 + 85 x_1_14
                        + 42 x_1_15 + 25 x_1_16 + 92 x_1_17 + 97 x_1_18
                        + 78 x_1_19 + 22 x_1_20 + 50 x_1_21 + 79 x_1_22
                        + 73 x_1_23 + 54 x_1_24 + 91 x_1_25 + 76 y_1_1
                        + 86 y_1_2 + 81 y_1_3 + 54 y_1_4 + 85 y_1_5 <= 825
 capacity_2:            70 x_2_1 + 90 x_2_2 + 58 x_2_3 + 72 x_2_4 + 100 x_2_5
                        + 24 x_2_6 + 90 x_2_7 + 56 x_2_8 + 15 x_2_9 + 49 x_2_10
                        + 60 x_2_11 + 98 x_2_12 + 81 x_2_13 + 85 x_2_14
                        + 42 x_2_15 + 25 x_2_16 + 92 x_2_17 + 97 x_2_18
                        + 78 x_2_19 + 22 x_2_20 + 50 x_2_21 + 79 x_2_22
                        + 73 x_2_23 + 54 x_2_24 + 91 x_2_25 + 76 y_2_1
                        + 86 y_2_2 + 81 y_2_3 + 54 y_2_4 + 85 y_2_5 <= 748
 max_one_bin_x_1:       x_1_1 + x_2_1 <= 1
 max_one_bin_x_2:       x_1_2 + x_2_2 <= 1
 max_one_bin_x_3:       x_1_3 + x_2_3 <= 1
 max_one_bin_x_4:       x_1_4 + x_2_4 <= 1
 max_one_bin_x_5:       x_1_5 + x_2_5 <= 1
 max_one_bin_x_6:       x_1_6 + x_2_6 <= 1
 max_one_bin_x_7:       x_1_7 + x_2_7 <= 1
 max_one_bin_x_8:       x_1_8 + x_2_8 <= 1
 max_one_bin_x_9:       x_1_9 + x_2_9 <= 1
 max_one_bin_x_10:      x_1_10 + x_2_10 <= 1
 max_one_bin_x_11:      x_1_11 + x_2_11 <= 1
 max_one_bin_x_12:      x_1_12 + x_2_12 <= 1
 max_one_bin_x_13:      x_1_13 + x_2_13 <= 1
 max_one_bin_x_14:      x_1_14 + x_2_14 <= 1
 max_one_bin_x_15:      x_1_15 + x_2_15 <= 1
 max_one_bin_x_16:      x_1_16 + x_2_16 <= 1
 max_one_bin_x_17:      x_1_17 + x_2_17 <= 1
 max_one_bin_x_18:      x_1_18 + x_2_18 <= 1
 max_one_bin_x_19:      x_1_19 + x_2_19 <= 1
 max_one_bin_x_20:      x_1_20 + x_2_20 <= 1
 max_one_bin_x_21:      x_1_21 + x_2_21 <= 1
 max_one_bin_x_22:      x_1_22 + x_2_22 <= 1
 max_one_bin_x_23:      x_1_23 + x_2_23 <= 1
 max_one_bin_x_24:      x_1_24 + x_2_24 <= 1
 max_one_bin_x_25:      x_1_25 + x_2_25 <= 1
 max_b_bin_y_1:         y_1_1 + y_2_1 <= 1
 max_b_bin_y_2:         y_1_2 + y_2_2 <= 2
 max_b_bin_y_3:         y_1_3 + y_2_3 <= 2
 max_b_bin_y_4:         y_1_4 + y_2_4 <= 1
 max_b_bin_y_5:         y_1_5 + y_2_5 <= 1
 dependent_decision_1:  x_1_1 - y_1_1 <= 0
 dependent_decision_2:  x_2_1 - y_2_1 <= 0
 dependent_decision_3:  x_1_6 - y_1_1 <= 0
 dependent_decision_4:  x_2_6 - y_2_1 <= 0
 dependent_decision_5:  x_1_16 - y_1_1 <= 0
 dependent_decision_6:  x_2_16 - y_2_1 <= 0
 dependent_decision_7:  x_1_2 - y_1_2 <= 0
 dependent_decision_8:  x_2_2 - y_2_2 <= 0
 dependent_decision_9:  x_1_8 - y_1_2 <= 0
 dependent_decision_10: x_2_8 - y_2_2 <= 0
 dependent_decision_11: x_1_11 - y_1_2 <= 0
 dependent_decision_12: x_2_11 - y_2_2 <= 0
 dependent_decision_13: x_1_12 - y_1_2 <= 0
 dependent_decision_14: x_2_12 - y_2_2 <= 0
 dependent_decision_15: x_1_15 - y_1_2 <= 0
 dependent_decision_16: x_2_15 - y_2_2 <= 0
 dependent_decision_17: x_1_3 - y_1_3 <= 0
 dependent_decision_18: x_2_3 - y_2_3 <= 0
 dependent_decision_19: x_1_9 - y_1_3 <= 0
 dependent_decision_20: x_2_9 - y_2_3 <= 0
 dependent_decision_21: x_1_19 - y_1_3 <= 0
 dependent_decision_22: x_2_19 - y_2_3 <= 0
 dependent_decision_23: x_1_20 - y_1_3 <= 0
 dependent_decision_24: x_2_20 - y_2_3 <= 0
 dependent_decision_25: x_1_21 - y_1_3 <= 0
 dependent_decision_26: x_2_21 - y_2_3 <= 0
 dependent_decision_27: x_1_24 - y_1_3 <= 0
 dependent_decision_28: x_2_24 - y_2_3 <= 0
 dependent_decision_29: x_1_4 - y_1_4 <= 0
 dependent_decision_30: x_2_4 - y_2_4 <= 0
 dependent_decision_31: x_1_10 - y_1_4 <= 0
 dependent_decision_32: x_2_10 - y_2_4 <= 0
 dependent_decision_33: x_1_17 - y_1_4 <= 0
 dependent_decision_34: x_2_17 - y_2_4 <= 0
 dependent_decision_35: x_1_18 - y_1_4 <= 0
 dependent_decision_36: x_2_18 - y_2_4 <= 0
 dependent_decision_37: x_1_22 - y_1_4 <= 0
 dependent_decision_38: x_2_22 - y_2_4 <= 0
 dependent_decision_39: x_1_25 - y_1_4 <= 0
 dependent_decision_40: x_2_25 - y_2_4 <= 0
 dependent_decision_41: x_1_5 - y_1_5 <= 0
 dependent_decision_42: x_2_5 - y_2_5 <= 0
 dependent_decision_43: x_1_7 - y_1_5 <= 0
 dependent_decision_44: x_2_7 - y_2_5 <= 0
 dependent_decision_45: x_1_13 - y_1_5 <= 0
 dependent_decision_46: x_2_13 - y_2_5 <= 0
 dependent_decision_47: x_1_14 - y_1_5 <= 0
 dependent_decision_48: x_2_14 - y_2_5 <= 0
 dependent_decision_49: x_1_23 - y_1_5 <= 0
 dependent_decision_50: x_2_23 - y_2_5 <= 0
Bounds
      x_1_1 = 1
      x_1_2 = 1
      x_1_3 = 0
 0 <= x_1_4 <= 1
 0 <= x_1_5 <= 1
      x_1_6 = 1
 0 <= x_1_7 <= 1
      x_1_8 = 0
      x_1_9 = 1
 0 <= x_1_10 <= 1
      x_1_11 = 1
      x_1_12 = 1
 0 <= x_1_13 <= 1
 0 <= x_1_14 <= 1
      x_1_15 = 1
      x_1_16 = 1
 0 <= x_1_17 <= 1
 0 <= x_1_18 <= 1
      x_1_19 = 1
      x_1_20 = 1
      x_1_21 = 1
 0 <= x_1_22 <= 1
 0 <= x_1_23 <= 1
      x_1_24 = 0
 0 <= x_1_25 <= 1
 0 <= x_2_1 <= 1
 0 <= x_2_2 <= 1
 0 <= x_2_3 <= 1
      x_2_4 = 0
      x_2_5 = 0
 0 <= x_2_6 <= 1
      x_2_7 = 1
 0 <= x_2_8 <= 1
 0 <= x_2_9 <= 1
      x_2_10 = 1
 0 <= x_2_11 <= 1
 0 <= x_2_12 <= 1
      x_2_13 = 1
      x_2_14 = 1
 0 <= x_2_15 <= 1
 0 <= x_2_16 <= 1
      x_2_17 = 0
      x_2_18 = 0
 0 <= x_2_19 <= 1
 0 <= x_2_20 <= 1
 0 <= x_2_21 <= 1
      x_2_22 = 1
      x_2_23 = 1
 0 <= x_2_24 <= 1
      x_2_25 = 1
      y_1_1 = 1
      y_1_2 = 1
      y_1_3 = 1
      y_1_4 = 0
 0 <= y_1_5 <= 1
 0 <= y_2_1 <= 1
      y_2_2 = 0
      y_2_3 = 0
      y_2_4 = 1
      y_2_5 = 1
End
