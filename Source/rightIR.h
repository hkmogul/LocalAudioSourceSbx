#define R_IRLEN 60
float rightIR[24][60] = {
{0.000021, 0.000045, 0.000187, -0.000217, 0.000253, -0.000290, 0.000382, -0.000241, 0.000383, -0.000189, 0.000505, -0.000407, 0.000945, -0.001493, -0.004810, -0.013964, -0.012673, -0.017214, -0.020234, -0.020535, 0.012164, 0.078000, -0.009250, -0.115184, 0.026399, -0.038465, -0.228138, -0.026524, 0.067081, 0.003726, 0.064967, 0.109014, 0.065269, 0.018739, 0.003606, 0.026024, -0.017579, -0.077462, -0.049319, 0.003427, 0.015159, 0.038232, 0.022495, -0.004969, 0.010774, -0.000643, -0.015749, -0.013371, -0.009347, 0.000218, -0.002253, 0.025767, 0.041883, 0.009389, 0.003596, 0.021397, 0.010355, 0.004175, -0.005616,-0.008891} , 
{0.000181, -0.000173, -0.000004, -0.000168, 0.000163, -0.000112, 0.000114, -0.000098, 0.000091, -0.000147, 0.000216, -0.000092, 0.000287, -0.000032, 0.000275, 0.000015, -0.001925, -0.006593, -0.010694, -0.012099, -0.014888, -0.015270, -0.011629, 0.036237, 0.037538, -0.054811, -0.043910, 0.010460, -0.103711, -0.105945, 0.005399, 0.022530, 0.023797, 0.056876, 0.065296, 0.034758, 0.005133, 0.012221, 0.015450, -0.025739, -0.048671, -0.022104, 0.002389, 0.018016, 0.026400, 0.004488, -0.003784, -0.000401, -0.005937, -0.008254, -0.010247, -0.002065, 0.002674, 0.011076, 0.026739, 0.018381, -0.000676, 0.003696, 0.011659,0.012376} , 
{0.000011, -0.000016, 0.000029, 0.000011, 0.000012, -0.000030, -0.000041, 0.000048, -0.000087, 0.000004, -0.000069, 0.000055, -0.000109, 0.000275, -0.000097, 0.001026, -0.000462, -0.000817, 0.000798, -0.001426, -0.005168, -0.006081, -0.007078, -0.011582, -0.012467, -0.010354, 0.016229, 0.026182, -0.025628, -0.035195, 0.003342, -0.058370, -0.084156, -0.015547, 0.003884, 0.006284, 0.033152, 0.045527, 0.030059, 0.012178, 0.011478, 0.015673, -0.007783, -0.030510, -0.020036, -0.003275, 0.003009, 0.013388, 0.003429, -0.007684, -0.004489, -0.003618, -0.002888, -0.000066, 0.004424, 0.002882, 0.004051, 0.015424, 0.014814,0.002927} , 
{0.000009, 0.000031, -0.000061, 0.000004, -0.000020, 0.000023, -0.000148, 0.000198, -0.000276, 0.000121, -0.000173, 0.000394, -0.000132, 0.000753, 0.000012, -0.001081, 0.000091, -0.000216, -0.000526, 0.001476, 0.000586, 0.000080, 0.000140, -0.000827, -0.004806, -0.006180, -0.007364, -0.009194, -0.011107, -0.003129, 0.015270, 0.008220, -0.022312, -0.013776, -0.012743, -0.055161, -0.050914, -0.013752, -0.001896, 0.005112, 0.025707, 0.032172, 0.019161, 0.011394, 0.013790, 0.006688, -0.011284, -0.019595, -0.015428, -0.011490, -0.003364, 0.002766, -0.006634, -0.005856, 0.003364, 0.007066, 0.008063, 0.012374, 0.009497,0.002067} , 
{0.000190, -0.000180, 0.000061, -0.000069, -0.000029, -0.000005, -0.000050, -0.000107, -0.000060, 0.000085, 0.000007, -0.000062, 0.000730, 0.000003, -0.001121, -0.000113, -0.000560, -0.000008, 0.001360, -0.000045, 0.000249, 0.001011, -0.000403, -0.000622, -0.000523, 0.000085, -0.000294, -0.000499, -0.002238, -0.004306, -0.005696, -0.007544, -0.008591, -0.006000, 0.006205, 0.011855, -0.010004, -0.017355, -0.009219, -0.037393, -0.051780, -0.021854, -0.005472, -0.001098, 0.015965, 0.025810, 0.016975, 0.005786, 0.002325, 0.001826, -0.008514, -0.019518, -0.017898, -0.008568, 0.000503, 0.002730, 0.001775, 0.007482, 0.012602,0.011662} , 
{-0.000011, 0.000023, 0.000089, 0.000006, -0.000038, -0.000023, -0.000226, 0.000055, -0.000100, 0.000213, -0.000116, 0.000572, 0.000571, -0.001293, -0.000687, 0.000043, -0.000331, 0.001410, -0.000106, 0.000546, 0.000939, -0.000754, -0.001140, 0.000153, -0.000349, 0.000467, -0.000348, 0.000272, 0.000132, 0.000207, -0.000243, -0.000579, -0.002353, -0.004152, -0.005596, -0.007511, -0.008621, -0.005852, 0.004482, 0.005265, -0.010528, -0.012605, -0.015924, -0.038567, -0.039853, -0.022608, -0.011043, -0.004145, 0.004128, 0.010385, 0.008696, 0.004040, 0.008693, 0.012690, 0.004340, -0.004993, -0.006277, -0.006827, -0.003847,0.001348} , 
{0.000043, -0.000019, -0.000062, -0.000000, -0.000010, -0.000025, -0.000162, 0.000132, -0.000079, 0.000147, -0.000095, 0.000387, 0.000660, -0.001426, -0.000923, 0.000262, 0.000015, 0.001065, 0.000070, 0.000834, 0.000307, -0.001371, -0.000610, 0.000077, 0.000421, 0.000112, -0.000471, 0.000360, 0.000231, -0.000003, -0.000255, 0.000096, -0.000466, -0.001211, -0.001814, -0.003570, -0.005275, -0.007188, -0.005998, -0.004252, -0.006852, -0.008556, -0.004687, -0.005839, -0.017395, -0.025459, -0.027225, -0.031256, -0.026609, -0.013013, -0.009428, -0.003142, 0.006697, 0.015102, 0.024215, 0.029250, 0.021965, 0.011417, 0.001236,-0.007424} , 
{-0.000061, -0.000054, 0.000101, -0.000114, 0.000095, -0.000166, 0.000103, -0.000244, 0.000217, -0.000129, 0.000221, -0.000208, 0.001042, -0.000585, -0.001317, -0.000331, 0.000219, 0.000702, 0.000479, 0.000500, 0.000482, -0.000997, -0.000860, -0.000016, 0.000639, 0.000208, -0.000498, -0.000212, 0.000401, -0.000214, -0.001022, -0.002345, -0.002980, -0.004604, -0.005353, -0.005316, -0.001170, 0.003675, -0.000481, -0.005060, -0.012143, -0.023373, -0.030238, -0.033666, -0.025867, 0.000984, 0.018720, 0.019661, 0.008954, -0.010340, -0.026100, -0.029237, -0.023061, -0.010780, 0.003058, 0.016839, 0.028062, 0.032034, 0.025150,0.012903} , 
{0.000163, -0.000127, 0.000194, -0.000115, 0.000178, -0.000191, 0.000041, -0.000250, 0.000049, 0.000011, 0.000024, 0.000000, -0.000010, 0.000660, -0.000190, -0.001325, -0.000335, 0.000077, 0.000822, 0.000282, 0.000453, 0.000257, -0.000563, -0.000762, 0.000351, 0.000714, -0.000020, -0.001368, -0.002375, -0.004506, -0.006443, -0.007501, -0.006920, -0.004274, 0.007314, 0.011316, -0.002470, -0.018757, -0.031753, -0.040539, -0.036904, -0.018796, 0.008875, 0.023905, 0.016588, 0.006418, -0.000715, -0.008149, -0.007845, -0.001861, 0.002958, 0.004642, 0.002750, -0.006577, -0.009281, -0.005445, -0.002066, 0.003954, 0.013679,0.014313} , 
{-0.000029, -0.000025, 0.000042, -0.000042, 0.000037, 0.000082, -0.000160, 0.000126, -0.000118, 0.000038, -0.000174, 0.000054, -0.000120, 0.000147, -0.000043, 0.000661, -0.000118, -0.000918, -0.000316, 0.000221, 0.000379, 0.000636, 0.000188, 0.000425, -0.001216, -0.002608, -0.006108, -0.007680, -0.010161, -0.009709, -0.007426, 0.006385, 0.022434, 0.004269, -0.021399, -0.041241, -0.059788, -0.053358, -0.022230, 0.016501, 0.035781, 0.024614, 0.015033, 0.013662, 0.006992, -0.002531, -0.013572, -0.014418, -0.002069, 0.006995, 0.007370, 0.013133, 0.013435, 0.006524, 0.006582, 0.004982, -0.005493, -0.014858, -0.016643,-0.009869} , 
{-0.000011, 0.000083, -0.000079, 0.000131, 0.000011, -0.000033, 0.000044, -0.000036, -0.000034, 0.000061, 0.000036, -0.000132, -0.000027, -0.000014, 0.000031, -0.000015, 0.000194, 0.000162, 0.000680, -0.000681, -0.000234, -0.000603, -0.002627, -0.007263, -0.010565, -0.012336, -0.012282, -0.010074, 0.003163, 0.036466, 0.018203, -0.032902, -0.053250, -0.075504, -0.078742, -0.029980, 0.029218, 0.048096, 0.039902, 0.027591, 0.019319, 0.017143, 0.002575, -0.022212, -0.024561, -0.011720, 0.000661, 0.008937, 0.014422, 0.020241, 0.018743, 0.015588, 0.012560, 0.004749, -0.008941, -0.015530, -0.009226, 0.000358, 0.000741,-0.005497} , 
{0.000066, -0.000132, 0.000071, -0.000052, -0.000067, -0.000082, 0.000004, 0.000005, -0.000087, 0.000084, 0.000027, -0.000022, 0.000164, 0.000002, 0.000226, 0.000020, 0.000226, 0.000053, -0.000483, -0.004003, -0.009985, -0.013674, -0.015612, -0.014731, -0.010644, 0.008428, 0.051670, 0.019139, -0.053445, -0.070755, -0.094485, -0.094324, -0.011564, 0.054593, 0.056092, 0.039679, 0.023358, 0.023320, 0.030685, 0.005600, -0.027152, -0.029647, -0.013743, 0.002674, 0.007248, 0.008660, 0.017625, 0.020431, 0.023021, 0.025648, 0.009629, -0.011807, -0.017592, -0.008364, 0.003209, 0.003854, -0.001223, -0.001051, 0.005640,0.011768} , 
{-0.000111, 0.000038, -0.000063, 0.000104, -0.000022, 0.000052, -0.000101, 0.000264, -0.000213, 0.000204, 0.000030, 0.000234, -0.000005, 0.000246, 0.000138, -0.000700, -0.005117, -0.012499, -0.016601, -0.018406, -0.016091, -0.012426, 0.013047, 0.064931, 0.016775, -0.064533, -0.088473, -0.116403, -0.104982, -0.003480, 0.071260, 0.071396, 0.050315, 0.016432, 0.015015, 0.043369, 0.020042, -0.029278, -0.037249, -0.021571, -0.003062, 0.007146, 0.016138, 0.019050, 0.016303, 0.025905, 0.030355, 0.014333, -0.007769, -0.017724, -0.009553, 0.004031, 0.004204, -0.005221, -0.003920, 0.008288, 0.019829, 0.022377, 0.016039,0.004162} , 
{-0.000001, 0.000054, -0.000014, 0.000186, -0.000134, 0.000288, -0.000090, 0.000352, -0.000017, 0.000463, -0.000003, 0.000356, -0.000632, -0.005560, -0.013832, -0.018341, -0.019000, -0.018511, -0.013668, 0.007891, 0.081243, 0.014668, -0.086812, -0.083224, -0.118488, -0.114922, 0.003945, 0.071713, 0.045866, 0.046031, 0.031817, 0.016701, 0.054031, 0.040900, -0.016314, -0.032024, -0.029361, -0.014372, -0.007582, 0.005013, 0.025929, 0.016329, 0.021657, 0.038255, 0.028344, -0.002171, -0.017302, -0.006185, 0.003995, 0.002347, -0.000995, -0.005034, -0.001041, 0.017513, 0.022424, 0.017180, 0.010129, 0.001551, -0.006654,-0.001161} , 
{0.000185, -0.000136, 0.000008, 0.000236, -0.000033, 0.000295, 0.000059, 0.000437, -0.000123, 0.000827, -0.003809, -0.010975, -0.021757, -0.018474, -0.021986, -0.012449, -0.013514, 0.064117, 0.089353, -0.087640, -0.124800, -0.102735, -0.136351, -0.034275, 0.100633, 0.070519, 0.012443, 0.018267, -0.001954, 0.034504, 0.093043, 0.016395, -0.033515, -0.011896, -0.017626, -0.025285, -0.018760, 0.023025, 0.032695, 0.016995, 0.033789, 0.030236, 0.003362, -0.008060, -0.005702, 0.004573, 0.010345, -0.001728, -0.006699, 0.001063, 0.015635, 0.021059, 0.016308, 0.010183, 0.004577, -0.007169, -0.006552, 0.004186, 0.000546,-0.005644} , 
{-0.000236, 0.000522, -0.000439, 0.000800, -0.000500, 0.001040, -0.000759, 0.001597, -0.005184, -0.013123, -0.026989, -0.019589, -0.025001, -0.008231, -0.014525, 0.084982, 0.111843, -0.128383, -0.154047, -0.138617, -0.156999, 0.007406, 0.152521, 0.114866, 0.017877, -0.014245, -0.045456, 0.000450, 0.107724, 0.030519, -0.030132, 0.014979, 0.005186, -0.019892, -0.025118, 0.004909, 0.012953, -0.001178, 0.026712, 0.042729, 0.017772, 0.000692, 0.014822, 0.017144, 0.003514, -0.011399, -0.019300, -0.005551, 0.019077, 0.024539, 0.019374, 0.014245, 0.007861, -0.004351, -0.009608, 0.003788, -0.000923, -0.010787, -0.006322,0.003010} , 
{-0.000778, 0.001267, -0.000748, 0.001239, -0.000831, 0.001725, -0.002547, -0.011368, -0.027607, -0.026627, -0.025522, -0.017099, -0.009349, 0.037291, 0.189117, -0.069890, -0.204592, -0.143606, -0.235063, -0.055780, 0.158270, 0.184931, 0.123169, 0.005822, -0.073008, -0.093748, 0.058282, 0.082608, -0.033299, 0.029184, 0.044925, -0.017880, -0.036717, -0.013696, 0.017578, -0.001025, 0.024486, 0.046108, 0.023972, -0.002888, 0.005109, 0.010087, 0.000001, -0.001019, -0.008799, -0.014979, 0.012314, 0.039203, 0.030786, 0.021805, 0.012313, -0.006175, -0.022453, -0.001329, 0.004084, -0.011139, -0.009910, 0.000002, 0.001467,0.000776} , 
{0.001429, -0.001243, 0.002139, -0.001806, 0.002901, -0.005894, -0.015023, -0.034067, -0.025873, -0.032397, -0.011185, -0.013351, 0.089582, 0.195100, -0.161237, -0.177345, -0.184086, -0.287546, 0.029099, 0.142570, 0.199483, 0.226392, 0.010169, -0.102340, -0.115913, 0.034180, 0.050654, -0.034345, 0.058755, 0.043004, -0.028257, -0.027546, 0.004770, 0.018382, -0.003536, 0.024035, 0.034298, 0.024496, 0.000574, -0.000873, 0.006752, 0.000746, 0.010194, -0.002182, -0.018029, 0.009169, 0.017690, 0.022683, 0.043447, 0.022203, -0.010099, -0.018308, -0.004201, -0.004164, -0.001759, -0.000621, -0.004254, -0.006023, -0.001427,-0.000062} , 
{0.001038, -0.000643, 0.001560, -0.001224, 0.002356, -0.008429, -0.019088, -0.036290, -0.026896, -0.036398, -0.008047, -0.013125, 0.144780, 0.143296, -0.199253, -0.129751, -0.250896, -0.278605, 0.098575, 0.054619, 0.213408, 0.330675, 0.017618, -0.080726, -0.111924, -0.033735, 0.027964, -0.014631, 0.035188, 0.036610, -0.018068, -0.026825, 0.028625, 0.031143, -0.008307, 0.015299, 0.021879, 0.030470, 0.003384, -0.014287, -0.009677, -0.005723, 0.025554, 0.015087, -0.006813, 0.018445, 0.020036, 0.025063, 0.032070, 0.005417, -0.018831, -0.023407, -0.006030, 0.004183, 0.002484, -0.007475, -0.009537, 0.000559, 0.001404,-0.001056} , 
{0.001179, -0.000769, 0.001667, -0.001337, 0.002507, -0.006962, -0.015746, -0.034307, -0.028221, -0.039240, -0.013899, -0.020264, 0.114179, 0.148618, -0.119741, -0.120223, -0.227159, -0.296891, 0.020918, 0.016072, 0.094540, 0.375200, 0.176074, -0.025883, -0.037721, -0.106388, -0.075961, 0.022775, -0.011563, 0.013137, 0.031042, -0.041031, 0.028466, 0.067624, -0.001974, 0.003153, 0.004313, 0.013474, 0.015750, -0.005908, -0.015462, -0.006967, 0.027013, 0.031697, 0.010050, 0.017040, 0.009278, 0.003242, 0.021745, 0.015602, -0.014372, -0.024618, -0.018429, -0.004055, 0.010781, 0.003266, -0.001337, 0.007357, 0.003774,-0.003219} , 
{-0.000429, 0.001011, -0.000513, 0.001568, -0.001032, 0.001973, -0.007328, -0.016130, -0.032384, -0.027465, -0.039489, -0.015416, -0.018646, 0.122872, 0.111862, -0.117326, -0.076753, -0.223580, -0.280296, -0.005468, 0.012273, 0.077885, 0.278618, 0.253438, 0.039741, -0.019434, -0.043974, -0.164518, -0.032035, 0.026774, -0.029887, 0.046217, -0.007619, 0.004146, 0.072867, 0.010301, -0.013810, -0.003091, 0.008242, 0.020675, 0.018638, 0.004685, -0.004440, 0.007638, 0.007762, 0.008745, 0.014435, 0.003952, 0.003079, 0.019173, 0.023459, 0.001140, -0.017700, -0.018809, -0.012464, 0.004301, 0.004578, -0.003477, 0.003557,0.004568} , 
{0.000200, 0.000031, 0.000387, -0.000141, 0.000657, -0.000203, 0.001052, -0.002869, -0.012422, -0.025515, -0.026253, -0.032475, -0.028278, -0.017820, 0.050359, 0.158918, -0.074372, -0.114579, -0.045489, -0.300373, -0.163729, 0.066508, 0.043639, 0.132865, 0.225842, 0.190227, -0.008133, -0.011226, -0.028705, -0.155550, -0.030410, 0.006328, -0.037150, 0.044869, 0.022657, 0.025271, 0.052191, 0.012109, -0.010921, 0.006828, 0.002457, -0.002880, 0.010089, -0.002635, -0.003060, 0.013625, 0.003516, 0.011545, 0.019522, 0.013245, 0.016925, 0.013718, 0.001245, -0.007565, -0.009566, -0.010136, -0.006808, 0.002797, -0.000033,-0.004081} , 
{0.000200, -0.000053, 0.000291, -0.000119, 0.000576, -0.000306, 0.000781, -0.000284, 0.001083, -0.004664, -0.014535, -0.024129, -0.020270, -0.030619, -0.025295, -0.013427, 0.085600, 0.120372, -0.158272, -0.082129, 0.034865, -0.326518, -0.157984, 0.150324, 0.047451, 0.077450, 0.172770, 0.161236, -0.003665, -0.035758, 0.031077, -0.084415, -0.102045, -0.015173, 0.004377, 0.043989, 0.051193, 0.018434, 0.005250, 0.008344, -0.006878, -0.011500, -0.012805, -0.009373, 0.021139, 0.007980, 0.012766, 0.036027, -0.001408, -0.008917, 0.021232, 0.022646, 0.015504, -0.001168, -0.011407, 0.002886, 0.005068, -0.008790, -0.006744,0.001297} , 
{-0.000073, 0.000188, 0.000038, 0.000051, 0.000109, 0.000172, 0.000159, 0.000098, 0.000575, -0.000327, 0.001084, -0.003376, -0.010163, -0.019987, -0.014760, -0.025358, -0.022761, -0.019606, 0.058095, 0.100881, -0.104231, -0.112672, 0.085415, -0.206310, -0.230277, 0.104801, 0.058756, 0.023837, 0.119983, 0.121430, 0.051991, -0.018049, 0.022718, 0.022160, -0.091673, -0.085494, 0.003352, 0.019374, 0.041229, 0.044663, -0.021868, -0.006648, 0.016869, -0.010001, -0.009637, -0.005750, 0.002932, 0.004102, 0.014619, 0.039181, 0.017161, -0.013084, 0.004797, 0.027873, 0.023675, 0.002287, -0.015254, -0.004251, 0.006986,-0.003067}}; 
int azimuth_r[24] = { 0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165, 180, 195, 210, 225, 240, 255, 270, 285, 300, 315, 330, 345};
