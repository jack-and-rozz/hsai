#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <random>
#include <fstream>
#include <chrono>

//8/4 use card value
// 80 to 15! so strong

//#define DEBUG
//#define RANDOM_PICK
//#define READ_EPOCH_FILE
//#define TEKAGEN
//#define DEBUG_PRINT

#ifdef READ_EPOCH_FILE
#include <dirent.h>
#include <cctype>
#include <algorithm>
#include <sys/stat.h>
#include <sys/types.h>
#endif

#ifdef DEBUG
#include<iostream>
#include<fstream>
#endif

double cardValues[160] = {0.000433529,-0.0037734,0.0028331,-0.00276243,0.00866409,0.00926344,0.029362,0.00190389,0.00771829,-0.0085139,0.00581557,-0.00253273,0.00805238,-0.0130036,0.00763868,-0.00624154,0.0089192,0.0130663,0.00519659,-0.00748882,0.00147536,-0.00139611,0.00551419,-0.00618836,0.00153689,0.000873843,0.00477187,0.000716053,0.00987119,-0.000576243,-0.00940427,0.00990177,0.00820396,-0.00255504,-0.0107639,0.000583625,0.0121412,-0.00073514,-0.00222392,-0.00431018,-0.00395841,-0.0183421,-0.00577225,0.0173889,-0.00909391,-0.00408303,0,0.00701637,0.0153841,0.00628554,0.0268336,0.00478309,0.0175815,0.00212425,-0.0197821,-0.00298329,-0.0164201,-0.0026735,-0.000324628,-0.00508684,0.00347833,-0.00198799,-0.00943521,0.00879462,0.0269608,0.0134584,0.0148968,0.0268221,0.0128387,-0.00183205,-0.0080639,0.00564646,0.0012535,-0.00336064,0.00918993,-0.0122763,0.000566452,-0.00734073,0.00065331,0.014409,0.00213692,0.00267892,-0.00134645,0.00707656,0.00295596,-0.0021922,0,0.00973341,-0.00481232,-0.00470274,0.00171294,-0.00863851,-3.54887e-05,-0.00495691,0.0114015,0.00413277,0.00348691,-0.00129351,0.00659267,-0.000100164,-0.0088968,-0.0131982,0.0147266,0.00447027,0.00678091,0.00324469,-0.0110531,-0.00837989,0.00685987,-0.029233,0.00775385,-0.00119425,-0.0179728,0.00456286,0.00147749,0.0126015,-0.00583768,-0.000873908,0.00163193,-0.00624224,0.00362457,0.00108682,-0.00808074,-0.0122458,-0.00222169,0.00195875,-0.00252306,0.00807992,0.00380313,-0.00646744,-0.00850858,-0.00819513,0.0121858,-0.00390422,0.00307388,-0.00422738,-0.00375989,-0.0140479,0.013371,-0.033683,0.0047868,-0.00601638,-0.0131875,0.00265587,0.00170743,-0.00901371,0.00466675,-0.000131648,-0.00772928,0.00934158,0.0150233,0.00444484,-0.0306787,-0.0165606,0.000195894,-0.0167981,0.00228041,0.00319721,-0.0113772,-0.0294801};

double cardWinrate[160][31]={{0.499477,0.50193,0.502039,0.5,0.569231,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499607,0.500721,0.507925,0.496042,0.485714,0.833333,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499045,0.502892,0.503045,0.511874,0.530067,0.482759,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500226,0.497428,0.495765,0.441558,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499337,0.502564,0.508964,0.529348,0.509091,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.49935,0.504071,0.499624,0.520039,0.540984,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.498146,0.498523,0.511123,0.522998,0.527929,0.520548,0.571429,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499597,0.50223,0.520724,0.486957,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499383,0.501831,0.500967,0.507909,0.518931,0.519231,0.8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.501206,0.491125,0.47027,0.493421,0.3125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499573,0.50125,0.501792,0.509327,0.471631,0.52381,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499803,0.502078,0.50103,0.464427,0.509434,0.6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499889,0.501271,0.498117,0.502934,0.440789,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500829,0.488089,0.467994,0.479452,0.75,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499782,0.501038,0.500041,0.511413,0.506667,0.4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500556,0.494705,0.478391,0.465116,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.49959,0.502732,0.497112,0.507324,0.547872,0.625,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.498753,0.505085,0.502999,0.507654,0.511494,0.47619,0.333333,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499682,0.501061,0.502069,0.500169,0.522491,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500687,0.494286,0.490896,0.494915,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499709,0.501376,0.502778,0.497969,0.454545,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500547,0.497469,0.493666,0.482399,0.411765,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499619,0.502076,0.499857,0.499745,0.500427,0.466667,0.4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500814,0.495689,0.493159,0.48505,0.467742,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499946,0.501001,0.495437,0.512563,0.529412,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.49962,0.504019,0.501531,0.573248,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499996,0.500283,0.496749,0.512438,0.666667,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499448,0.506252,0.502607,0.505747,0.444444,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.498801,0.504672,0.518742,0.499564,0.445783,0.8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499847,0.500656,0.501733,0.498601,0.533333,0.4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500887,0.4925,0.482891,0.475655,0.555556,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499085,0.502093,0.504345,0.505287,0.509532,0.48227,0.428571,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499399,0.504299,0.497908,0.500531,0.527174,0.333333,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500528,0.497846,0.489205,0.501266,0.490566,0.666667,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500907,0.490456,0.477273,0.423729,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500167,0.500803,0.491641,0.486976,0.428571,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499583,0.499777,0.503033,0.513215,0.489749,0.454545,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499605,0.502727,0.501006,0.486004,0.44385,0.666667,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500019,0.499383,0.501949,0.498219,0.520548,0.428571,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.50072,0.495804,0.491254,0.478723,0.390244,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.50072,0.493991,0.499577,0.495238,0.520833,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.501351,0.477737,0.440257,0.444444,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.501331,0.491211,0.472744,0.441281,0.307692,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.49965,0.500376,0.501608,0.500739,0.51506,0.510101,0.444444,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.50142,0.486654,0.464247,0.443662,0.6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500975,0.496955,0.484747,0.455621,0.321429,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.49993,0.501851,0.49062,0.472826,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.498078,0.500463,0.514439,0.517003,0.526062,0.528302,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.498173,0.503797,0.514153,0.540484,0.53169,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499276,0.500991,0.503705,0.5157,0.506241,0.532468,0.6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499241,0.498404,0.50388,0.511643,0.52379,0.51005,0.53125,0.666667,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499607,0.501838,0.505014,0.497517,0.519481,0.666667,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.498288,0.50214,0.510261,0.516334,0.511818,0.546218,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499768,0.50136,0.501938,0.488943,0.512821,0.545455,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.501478,0.474887,0.430008,0.396552,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500409,0.496861,0.483948,0.504762,0.5,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.501415,0.478542,0.430611,0.298246,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.501175,0.491945,0.490638,0.477583,0.615385,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500421,0.50036,0.494876,0.491586,0.488,0.4375,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.501281,0.496416,0.487247,0.488442,0.541935,0.384615,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.49997,0.501055,0.499406,0.491568,0.451264,0.533333,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500911,0.499059,0.485312,0.483584,0.445946,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500453,0.49389,0.473242,0.466667,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499014,0.504399,0.512195,0.509783,0.5,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.49824,0.500946,0.505975,0.527596,0.516657,0.551887,0.583333,0.666667,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499262,0.501056,0.502551,0.508395,0.519805,0.445545,0.25,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.498578,0.50246,0.507227,0.50796,0.508958,0.478632,0.75,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.49829,0.500105,0.509879,0.509256,0.52072,0.47619,0.534247,0.142857,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.498992,0.499231,0.508604,0.50718,0.525786,0.53252,0.222222,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500166,0.498165,0.499219,0.517241,0.25,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500625,0.491855,0.492063,0.471264,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500709,0.494474,0.502903,0.490248,0.551282,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500369,0.497648,0.495236,0.466258,0.68,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500574,0.496941,0.491692,0.48806,0.55,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500155,0.499028,0.502045,0.494464,0.481833,0.53125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.501201,0.487615,0.456595,0.456311,0.25,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500323,0.500372,0.496999,0.490965,0.489564,0.321429,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500978,0.491275,0.476017,0.435115,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500002,0.503004,0.483757,0.459552,0.541667,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.498864,0.502092,0.50449,0.51118,0.512968,0.386364,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500082,0.499771,0.500171,0.496636,0.473913,0.636364,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499865,0.500264,0.500694,0.501416,0.488636,0.575342,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499551,0.503559,0.500865,0.508772,0.375,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.498717,0.507896,0.515173,0.493976,0.657143,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499549,0.502263,0.500659,0.504074,0.492625,0.6,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500395,0.500423,0.482525,0.498468,0.517241,0.571429,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499904,0.499917,0.506144,0.52139,0.45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499644,0.499983,0.503909,0.50435,0.486376,0.5625,0.666667,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500071,0.500193,0.498986,0.446721,0.5,0.333333,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500304,0.498076,0.490323,0.476684,0.25,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499493,0.502816,0.503742,0.493789,0.583333,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.501388,0.493049,0.476678,0.452525,0.458333,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499771,0.499801,0.500179,0.52746,0.52459,0.333333,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500083,0.49842,0.503609,0.518519,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499234,0.501359,0.514896,0.506369,0.520325,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.49932,0.504076,0.515262,0.550314,0.545455,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.49968,0.500904,0.506164,0.481311,0.523121,0.461538,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499579,0.503489,0.497537,0.495327,0.51087,0.75,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499054,0.502299,0.505226,0.506546,0.532925,0.454545,0.666667,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500081,0.499895,0.498621,0.493719,0.531034,0.333333,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500542,0.491848,0.49023,0.510638,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500889,0.488184,0.461591,0.449275,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499046,0.503402,0.502967,0.505059,0.507788,0.47619,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500111,0.499212,0.500698,0.492754,0.491228,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499895,0.499657,0.503422,0.494373,0.511013,0.516129,0.666667,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499768,0.50087,0.503238,0.492726,0.483871,0.642857,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.501106,0.48939,0.461538,0.484127,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500957,0.493326,0.475117,0.448718,0.333333,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499887,0.501288,0.496423,0.50695,0.509719,0.5,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.502042,0.467122,0.439344,0.46,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.50011,0.500248,0.498065,0.49625,0.50565,0.421053,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.50025,0.498797,0.49779,0.475831,0.521739,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.501378,0.480382,0.465318,0.265306,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.50007,0.500969,0.497464,0.499148,0.499496,0.469136,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499814,0.501581,0.498361,0.497959,0.333333,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.498755,0.502756,0.504157,0.508169,0.495843,0.422764,0.25,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.501075,0.495085,0.488705,0.474359,0.369565,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499536,0.504004,0.494656,0.53719,0.625,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499688,0.50279,0.498173,0.495852,0.476064,0.45,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500706,0.491315,0.460307,0.405797,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499847,0.50289,0.492063,0.454248,0.647059,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499938,0.500727,0.50174,0.473684,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.501143,0.481339,0.469922,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.501719,0.482463,0.452921,0.463855,0.4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500308,0.499179,0.494224,0.481203,0.565789,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499513,0.502259,0.503398,0.495315,0.497992,0.578947,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.50052,0.49801,0.490455,0.496475,0.434783,0.6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.49948,0.503856,0.499681,0.486594,0.506173,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499644,0.502158,0.505643,0.486859,0.446809,0.285714,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500505,0.496086,0.464442,0.460177,0.666667,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500858,0.488672,0.472298,0.483516,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.501507,0.487819,0.475117,0.491039,0.583333,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499166,0.502265,0.504459,0.505153,0.477523,0.416667,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500275,0.497811,0.490323,0.480583,0.375,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499933,0.499286,0.507928,0.473014,0.516393,0.25,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500098,0.499766,0.489953,0.54321,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500279,0.497411,0.474457,0.558824,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.50094,0.486673,0.457986,0.389831,0.333333,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.498592,0.50389,0.507847,0.517241,0.487755,0.520833,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.502689,0.457402,0.379556,0.285714,0.333333,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499127,0.503522,0.50502,0.536772,0.446154,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500063,0.501574,0.473583,0.483333,0.666667,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.501623,0.478688,0.446755,0.461538,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499055,0.503113,0.500658,0.523475,0.477927,0.538462,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499636,0.501209,0.503333,0.507469,0.46798,0.571429,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500141,0.498816,0.486686,0.506024,0.285714,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499629,0.503207,0.503744,0.480243,0.615385,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499439,0.507516,0.498212,0.5,0.571429,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500412,0.496782,0.492948,0.485549,0.625,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499319,0.504114,0.502989,0.507109,0.405063,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.498505,0.499551,0.509093,0.515699,0.534028,0.565714,0.5,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499971,0.498999,0.504176,0.496117,0.487032,0.431818,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.502068,0.460479,0.429767,0.333333,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.501161,0.481694,0.457298,0.365079,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499476,0.503333,0.500912,0.490954,0.614286,0.5,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.502131,0.471312,0.456094,0.386139,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500038,0.500812,0.497973,0.489861,0.496032,0.5,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.499226,0.500541,0.509791,0.517945,0.506452,0.703704,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.500221,0.496714,0.491815,0.453125,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0.502242,0.455906,0.417674,0.545455,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
};

//TODO think about card advantage
//TODO 

// 0805 honkide 13th

using namespace std;

double** mulMatrix(double** a,double** b, int r1, int c1, int r2, int c2)
{
    // If column of first matrix in not equal to row of second matrix,
    // ask the user to enter the size of matrix again.
    while (c1!=r2)
    {
        cerr << "Error! column of first matrix not equal to row of second.";
    }

    cerr << "1" << endl;

    double** result = new double*[r1];
    for(int i = 0; i < r1; i++){
        result[i] = new double[c2];
    }

    cerr << "2" << endl;

    // Initializing elements of matrix mult to 0.
    for(int i = 0; i < r1; i++){
        for(int j = 0; j < c2; j++)
        {
            result[i][j]=0;
        }
    }

    cerr << "3" << endl;

    // Multiplying matrix a and b and storing in array mult.
    for(int i = 0; i < r1; i++){
        for(int j = 0; j < c2; j++){
            for(int k = 0; k < c1; k++)
            {
                /*cerr << i << ":" << j << ":" << k << endl;
                cerr << result[i][j] << endl;
                cerr << a[i][k] << endl;
                cerr << b[k][j] << endl;*/
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    // Displaying the multiplication of two matrix.
    //cerr << "Output Matrix: " << endl;
    /*for(int i = 0; i < r1; i++)
    for(int j = 0; j < c2; j++)
    {
        //cerr << " " << result[i][j];
        if(j == c2-1)
            //cerr << endl;
    }*/
    return result;
}

std::vector<std::string> split(std::string str, char del) {
    int first = 0;
    int last = str.find_first_of(del);
 
    std::vector<std::string> result;
 
    while (first < str.size()) {
        std::string subStr(str, first, last - first);
 
        result.push_back(subStr);
 
        first = last + 1;
        last = str.find_first_of(del, first);
 
        if (last == std::string::npos) {
            last = str.size();
        }
    }
 
    return result;
}

class Card {
    int cardNumber;
    int instanceId;
    int location;
    int cardType;
    int cost;
    int attack;
    int defense;
    string abilities;
    int myHealthChange;
    int opponentHealthChange;
    int cardDraw;
    bool actionDone;
    int playScore;
    string command;
public:
    Card(int cardNumber, int instanceId, int location, int cardType, int cost, int attack, int defense, string abilities, int myHealthChange, int opponentHealthChange, int cardDraw, bool actionDone){
        this->cardNumber = cardNumber;
        this->instanceId = instanceId;
        this->location = location;
        this->cardType = cardType;
        this->cost = cost;
        this->attack = attack;
        this->defense = defense;
        this->abilities = abilities;
        this->myHealthChange = myHealthChange;
        this->opponentHealthChange = opponentHealthChange;
        this->cardDraw = cardDraw;
        this->actionDone = actionDone;
        this->playScore = 0;
    }

    Card* getCardCopy(){
        return new Card(this->cardNumber, this->instanceId, this->location, this->cardType, this->cost, this->attack, this->defense, this->abilities, this->myHealthChange, this->opponentHealthChange, this->cardDraw, this->actionDone);
    }

    int getDraw(){
        return cardDraw;
    }

    int getCost(){
        return cost;
    }

    int getID(){
        return instanceId;
    }

    int getAttack(){
        return attack;
    }

    int getDefense(){
        return defense;
    }

    string getAbilities(){
        return abilities;
    }

    bool isGuard(){
        return abilities.find("G") != std::string::npos;
    }

    bool isCharge(){
        return abilities.find("C") != std::string::npos;
    }

    bool isLethal(){
        return abilities.find("L") != std::string::npos;
    }

    bool isWard(){
        return abilities.find("W") != std::string::npos;
    }

    bool isDrain(){
        return abilities.find("D") != std::string::npos;
    }

    bool isBreakThrough(){
        return abilities.find("B") != std::string::npos;
    }

    bool getActionDone(){
        return actionDone;
    }

    void doAction(){
        this->actionDone = true;
    }

    int takeDamage(int damage){
        if(isWard()){
            std::replace(abilities.begin(), abilities.end(), 'W', '-');
        }
        else{
            defense -= damage;
        }
    }

    // hyokati calculation
    int calcBasicValue(){
        int base = attack + defense;
        if(isWard()){
            base += attack;
        }
        if(isGuard()){
            base += 1;
        }
        if(isDrain()){
            base += attack;
        }
        if(isBreakThrough()){
            if(attack >= 5){
                base += attack - 4;
            }
        }
        return base;
    }

    int getType(){
        return cardType;
    }

    bool operator<(const Card& another) const {
        return cost > another.cost;
    }

    int getPlayScore(){
        return playScore;
    }

    void setPlayScore(int score){
        this->playScore = score;
    }

    void buff(Card* card){
        if(card->isGuard() && !isGuard()){
            abilities += 'G';
        }
        if(card->isLethal() && !isLethal()){
            abilities += 'L';
        }
        if(card->isWard() && !isWard()){
            abilities += 'W';
        }
        if(card->isDrain() && !isDrain()){
            abilities += 'D';
        }
        if(card->isBreakThrough() && !isBreakThrough()){
            abilities += 'B';
        }
        attack += card->getAttack();
        defense += card->getDefense();
    }

    void deBuff(Card* card){
        if(card->isGuard()){
            std::replace(abilities.begin(), abilities.end(), 'G', '-');
        }
        if(card->isLethal()){
            std::replace(abilities.begin(), abilities.end(), 'L', '-');
        }
        if(card->isWard()){
            std::replace(abilities.begin(), abilities.end(), 'W', '-');
        }
        if(card->isDrain()){
            std::replace(abilities.begin(), abilities.end(), 'D', '-');
        }
        if(card->isBreakThrough()){
            std::replace(abilities.begin(), abilities.end(), 'B', '-');
        }
        attack += card->getAttack();
        if(attack < 0){
            attack = 0;
        }
        takeDamage(-card->getDefense());
    }

    void setCommand(string command){
        this->command = command;
    }

    string getCommand(){
        return command;
    }

    int getCardNumber(){
        return cardNumber;
    }

    int getMyHealthChange(){
        return myHealthChange;
    }

    int getOpponentHealthChange(){
        return opponentHealthChange;
    }
};
bool cmp(Card *a, Card *b)
    {
        return (a->getCost() > b->getCost()); 
    } 
class Player {
    int health;
    int mana;
    int deck;
    int rune;
public:
    Player(int health, int mana, int deck, int rune){
        this->health = health;
        this->mana = mana;
        this->deck = deck;
        this->rune = rune;
    }

    Player* getPlayerCopy(){
        return new Player (health, mana, deck, rune);
    }

    int getMana(){
        return mana;
    }

    int getHealth(){
        return health;
    }

    int takeDamage(int damage){
        health -= damage;
    }
};


int getTradeScore(Card* me, Card* target){
    int dealDamage = min(me->getAttack(), target->getDefense());
    if( target->isWard()){
        dealDamage = 1 + target->getAttack();
    }
    int takeDamage = min(target->getAttack(), me->getDefense());
    if( me->isWard()){
        takeDamage = 1;
    }
    int score = dealDamage - takeDamage;
    if(me->getAttack() >= target->getDefense() || (me->isLethal() && dealDamage > 0)){
        score += target->getDefense() + target->getAttack();
    }
    if(target->getAttack() >= me->getDefense() || (target->isLethal() && takeDamage > 0)){
        score -= me->getDefense() + me->getAttack();
    }
    return score;
}

int executeFight(Card* attacker, Card* target){
    attacker->doAction();
    if(attacker->isLethal()){
        target->takeDamage(999);
    }
    else{
        target->takeDamage(attacker->getAttack());
    }
    if(target->isLethal()){
        attacker->takeDamage(999);
    }
    else{
        attacker->takeDamage(target->getAttack());
    }
}

int LEATHAL_DANGER_VALUE = 7;

double getBoardScore(vector<Card*> myBoardCard, vector<Card*> enemyBoardCard, Player* me, Player* enemy){
    // enemy lethal
    int lethalScore = 0;
    bool hasTaunt = false;
    for(Card* enemyCard: enemyBoardCard){
        if(enemyCard->isGuard()){
            hasTaunt = true;
            break;
        }
    }
    int totalAttack = 0;
    int nextTotalAttack = 0;
    int lifeStealTotal = 0;
    for(Card* card: myBoardCard){
        nextTotalAttack += card->getAttack();
        if(!card->getActionDone()){
            totalAttack += card->getAttack();
        }
    }
    for(Card* card: enemyBoardCard){
        if(card -> isDrain()){
            lifeStealTotal += card->getAttack();
        }
    }
    if(!hasTaunt && totalAttack >= enemy->getHealth() || enemy->getHealth() <= 0){
        lethalScore = 10000;
    }
    else if(!hasTaunt && totalAttack + nextTotalAttack + LEATHAL_DANGER_VALUE >= enemy->getHealth() + lifeStealTotal){
        lethalScore = totalAttack + nextTotalAttack + LEATHAL_DANGER_VALUE - enemy->getHealth() - lifeStealTotal;
    }

    // next turn leathal score


    // min max algorizm
    // enemy card value is max(enemy card, max tradable my card)
    // my card value is max(my card, min tradable enemy card)
    int myCardValueTotal = 0;
    int myDefenceTotal = 0;
    for(Card* myCard: myBoardCard){
        int minTradeScore = -999;
        for(Card* enemyCard: enemyBoardCard){
            int score = getTradeScore(myCard, enemyCard);
            if(minTradeScore > score){
                minTradeScore = score;
            }
        }
        if(myCard->isGuard()){
            myDefenceTotal += myCard->getDefense();
        }
        int basicValue = myCard->calcBasicValue();
        /*if(myCard->isLethal()){
            basicValue += me->getMana();
        }*/
        if(minTradeScore != -999){
            myCardValueTotal += max(basicValue, minTradeScore);
        }
        else{
            myCardValueTotal += basicValue;
        }
    }

    int enemyCardValueTotal = 0;
    int enemyAttackTotal = 0;
    for(Card* enemyCard: enemyBoardCard){
        int maxTradeScore = 0;
        for(Card* myCard: myBoardCard){
            int score = getTradeScore(enemyCard, myCard);
            if(maxTradeScore < score){
                maxTradeScore = score;
            }
        }
        int basicValue = enemyCard->calcBasicValue();
        /*if(enemyCard->isLethal()){
            basicValue += me->getMana();
        }*/
        enemyAttackTotal += enemyCard->getAttack();
        enemyCardValueTotal += max(basicValue, maxTradeScore);
    }
    // avoid enemy leathal
    int leathalPenalty = 0;
    if(enemyAttackTotal >= me->getHealth() + myDefenceTotal - LEATHAL_DANGER_VALUE){
        leathalPenalty = (enemyAttackTotal - myDefenceTotal - me->getHealth() + LEATHAL_DANGER_VALUE + 1) * 100;
    }

    return myCardValueTotal - enemyCardValueTotal - leathalPenalty + lethalScore;
}

class SaikiResult{
public:
    vector<Card*> useCards;
    int score;

    SaikiResult(vector<Card*> useCards, int score){
        this->useCards = useCards;
        this->score = score;
    }
};

class Game {
    vector<Card*> myHandCard;
    vector<Card*> myBoardCard;
    vector<Card*> enemyBoardCard;
    Player* me;
    Player* opponent;
    std::stringstream ss;
    vector<Game*> nextGames;

    int manaLeft;
public:
    Game(vector<Card*> myHandCard, vector<Card*> myBoardCard, vector<Card*> enemyBoardCard, Player* me, Player* opponent, string ss, int manaLeft){
        this->myHandCard = myHandCard;
        this->myBoardCard = myBoardCard;
        this->enemyBoardCard = enemyBoardCard;
        this->me = me;
        this->opponent = opponent;
        this->ss << ss;
        this->manaLeft = manaLeft;
    }

    vector<Game*> getNextGames(){
        if(nextGames.size() > 0){
            return nextGames;
        }
        else{
            // 貪欲だが、1階層目だけ展開する

            int maxScore = -99999999;
            Game* resultGame = nullptr;
            int currentBoardScore = getBoardScore(myBoardCard, enemyBoardCard, me, opponent);

            for(Card* card: myBoardCard){
                // deal yuuri trade
                if(card->getAttack() == 0 || card->getActionDone()){
                    continue;
                }
                Card* targetCard = nullptr;
                bool hasTaunt = false;
                // taunt
                for(Card* enemyCard: enemyBoardCard){
                    if(enemyCard->isGuard()){
                        hasTaunt = true;
                        Game* simulateResult = simulateTrade(card, enemyCard);
                        nextGames.push_back(simulateResult);
                    }
                }
                if (!hasTaunt) {
                    // calculate face attack value
                    Game* simulateResult = simulateTrade(card, nullptr);
                    nextGames.push_back(simulateResult);

                    for(Card* enemyCard: enemyBoardCard){
                        Game* simulateResult = simulateTrade(card, enemyCard);
                        nextGames.push_back(simulateResult);
                    }
                }
            }
        }
        return nextGames;
    }

    Game* originalSimulation(bool goToFace){

        // 貪欲だが、1階層目だけ展開する

        //int maxScore = -99999999;
        int maxScore = -1;
        Game* resultGame = nullptr;

        for(Game* game: getNextGames()){
            vector<Game*> finalGames = game->getNextGames();
            for(Game* finalGame: finalGames){
                int score = finalGame->getGameBoardScore();
                if(score > maxScore){
                    maxScore = score;
                    resultGame = game;
                }
            }
        }

        // PASS
        if(resultGame == nullptr){
            resultGame = getCopy();
            resultGame->addString("PASS;");
        }

        return resultGame;
    }

    Game* simulation(bool goToFace){

        int maxScore = -99999999;
        Game* resultGame = nullptr;

        for(Card* card: myBoardCard){
            // deal yuuri trade
            if(card->getAttack() == 0 || card->getActionDone()){
                continue;
            }
            Card* targetCard = nullptr;
            bool hasTaunt = false;
            // taunt
            for(Card* enemyCard: enemyBoardCard){
                if(enemyCard->isGuard()){
                    hasTaunt = true;
                    Game* simulateResult = simulateTrade(card, enemyCard);
                    int score = simulateResult->getGameBoardScore();
                    if(score > maxScore){
                        maxScore = score;
                        resultGame = simulateResult;
                        if(maxScore > 9000){
                            return resultGame;
                        }
                    }
                }
            }
            if (!hasTaunt) {
                // calculate face attack value
                Game* simulateResult = simulateTrade(card, nullptr);
                int faceScore = simulateResult->getGameBoardScore();
                if(faceScore > maxScore){
                    maxScore = faceScore;
                    resultGame = simulateResult;
                    if(maxScore > 9000){
                        return resultGame;
                    }
                }

                for(Card* enemyCard: enemyBoardCard){
                    Game* simulateResult = simulateTrade(card, enemyCard);
                    int score = simulateResult->getGameBoardScore();
                    if(score > maxScore){
                        maxScore = score;
                        resultGame = simulateResult;
                        if(maxScore > 9000){
                            return resultGame;
                        }
                    }
                }
            }
        }
        
        // PASS
        if(resultGame == nullptr){
            resultGame = getCopy();
            resultGame->addString("PASS;");
        }

        return resultGame;
    }


Game* simulateTrade(Card* attacker, Card* target){
    vector<Card*> myBoardCardCopy;
    vector<Card*> enemyBoardCardCopy;
    Player* meCopy = me->getPlayerCopy();
    Player* enemyCopy = opponent->getPlayerCopy();
    std::stringstream ssCopy;
    ssCopy << ss.str(); // copy everything inside ss1's buffer to ss2's buffer
    // make copy board
    for(Card* card: myBoardCard){
        if(attacker->getID() == card->getID()){
            attacker = card->getCardCopy();
            myBoardCardCopy.push_back(attacker);
        }
        else{
            myBoardCardCopy.push_back(card);
        }
    }
    for(Card* card: enemyBoardCard){
        if(target != nullptr && target->getID() == card->getID()){
            target = card->getCardCopy();
            enemyBoardCardCopy.push_back(target);
        }
        else{
            enemyBoardCardCopy.push_back(card);
        }
    }
    if(target != nullptr){
        if(attacker->isDrain() && !target->isWard()){
            meCopy->takeDamage(-attacker->getAttack());
        }
        executeFight(attacker, target);
        // trunple
        if(attacker->isBreakThrough()){
            if(target->getDefense() < 0){
                enemyCopy->takeDamage(-target->getDefense());
            }
        }
    }
    // face
    else {
        if(attacker->isDrain()){
            meCopy->takeDamage(-attacker->getAttack());
        }
        attacker->doAction();
        enemyCopy->takeDamage(attacker->getAttack());
    }
    // jyoukyou kiin syori
    auto itr = myBoardCardCopy.begin();
    while (itr != myBoardCardCopy.end())
    {
        if ((*itr)->getDefense() <= 0)
        {
            itr = myBoardCardCopy.erase(itr);
        }
        else
        {
            itr++;
        }
    }

    itr = enemyBoardCardCopy.begin();
    while (itr != enemyBoardCardCopy.end())
    {
        if ((*itr)->getDefense() <= 0)
        {
            itr = enemyBoardCardCopy.erase(itr);
        }
        else
        {
            itr++;
        }
    }
    if(target != nullptr){
        ssCopy << "ATTACK " << attacker->getID() << " " << target->getID() << ";";
    }
    else{
        ssCopy << "ATTACK " << attacker->getID() << " " << -1 << ";";
    }
    return new Game(myHandCard, myBoardCardCopy, enemyBoardCardCopy, meCopy, enemyCopy, ssCopy.str(), manaLeft);
}


    Game* getCopy(){
        vector<Card*> myHandCardCopy;
        vector<Card*> myBoardCardCopy;
        vector<Card*> enemyBoardCardCopy;
        Player* meCopy;
        Player* opponentCopy;
        std::stringstream ssCopy;
        ssCopy << ss.str(); // copy everything inside ss1's buffer to ss2's buffer

        // make copy board
        for(Card* card: myHandCard){
            myHandCardCopy.push_back(card->getCardCopy());
        }
        for(Card* card: myBoardCard){
            myBoardCardCopy.push_back(card->getCardCopy());
        }
        for(Card* card: enemyBoardCard){
            enemyBoardCardCopy.push_back(card->getCardCopy());
        }
        meCopy = me->getPlayerCopy();
        opponentCopy = opponent->getPlayerCopy();
        return new Game(myHandCardCopy, myBoardCardCopy, enemyBoardCardCopy, meCopy, opponentCopy, ssCopy.str(), manaLeft);
    }


    SaikiResult* saikiPlayCard(int index, vector<Card*> currentPlay, int leftMana, int boardLeft, Game* current){
        SaikiResult* result = new SaikiResult(currentPlay, 0);
        vector<Card*> playCard;
        
        if(myHandCard.size() <= index){
            Game* copyGame = current->getCopy();
            int score = 0;
            int totalDraw = 0;
            for(Card* card: currentPlay){
                totalDraw += card->getDraw();
                // use health only cards
                if(card->getType() == 3 && card->getDefense() == 0){
                    totalDraw += 2;
                }
            }
            // simulate trade
            while(copyGame->getCommands().find("PASS") == std::string::npos){
                copyGame = copyGame->simulation(false);
            }
            result->score = copyGame->getGameBoardScore() - currentPlay.size() + totalDraw;
            return result;
        }
        result = saikiPlayCard(index + 1, currentPlay, leftMana, boardLeft, current);
        if(boardLeft == 0 && myHandCard[index]->getType() == 0){
            return result;
        }

        if(myHandCard[index]->getCost() <= leftMana){
            if(myHandCard[index]->getType() == 0){
                boardLeft --;   
            }
            vector<Card*> newPlay;
            SaikiResult* tmpResult;
            int tmpScore = 0;
            for(Card* card: currentPlay){
                newPlay.push_back(card);
            }
            Card* playCardCopy = myHandCard[index]->getCardCopy();
            newPlay.push_back(playCardCopy);
            if(playCardCopy->getType() == 0){
                boardLeft --;
            }

            Game* newBoard = current->getCopy();
            newBoard->setBestCardPlay(playCardCopy, newBoard->getGameBoardScore());
            newBoard->playCard(playCardCopy);

            tmpResult = saikiPlayCard(index + 1, newPlay, leftMana - myHandCard[index]->getCost(), boardLeft, newBoard);
            if(tmpResult->score > result->score){
                result = tmpResult;
            }
        }
        
        return result;
    }

    int getGameBoardScore(){
        return getBoardScore(myBoardCard, enemyBoardCard, me, opponent);
    }

    vector<Card*> calcBestPlayCards(bool goToFace){
        vector<Card*> firstCards;
        return saikiPlayCard(0, firstCards, manaLeft, 6 - myBoardCard.size(), this)->useCards;
    }

    void setBestCardPlay(Card* hand, int currentBoardScore){
        // health change
        Player* copyMe = me->getPlayerCopy();
        Player* copyOpponent = opponent->getPlayerCopy();
        copyMe->takeDamage(-hand->getMyHealthChange());
        copyOpponent->takeDamage(-hand->getOpponentHealthChange());
        
        if(hand->getType() == 1){
            if(myBoardCard.size() != 0){
                int maxScore = -99999;
                for(Card* targetCreature : myBoardCard){
                    vector<Card*> myBoardCardCopy;
                    for(Card* card: myBoardCard){
                        if(targetCreature->getID() == card->getID()){
                            Card* buffedCard = card->getCardCopy();
                            buffedCard->buff(hand);
                            myBoardCardCopy.push_back(buffedCard);
                        }
                        else{
                            myBoardCardCopy.push_back(card);
                        }
                    }
                    Game* current = new Game(myHandCard, myBoardCardCopy, enemyBoardCard, copyMe, copyOpponent, "", manaLeft);
                    while(current->getCommands().find("PASS") == std::string::npos){
                        current = current->simulation(false);
                    }
                    int score = current->getGameBoardScore() - currentBoardScore - 1 + hand->getDraw();
                    if(score > maxScore){
                        maxScore = score;
                        hand->setPlayScore(score);
                        std::stringstream ss;
                        ss << "USE " << hand->getID() << " " << targetCreature->getID() << ";";
                        hand->setCommand(ss.str());
                    }
                }
            }
            else{
                hand->setPlayScore(-999);
            }
        }
        else if(hand->getType() == 2){
            if(enemyBoardCard.size() != 0){
                int maxScore = -99999;
                for(Card* targetCreature : enemyBoardCard){
                    vector<Card*> enemyBoardCardCopy;
                    for(Card* card: enemyBoardCard){
                        if(targetCreature->getID() == card->getID()){
                            Card* buffedCard = card->getCardCopy();
                            buffedCard->deBuff(hand);
                            enemyBoardCardCopy.push_back(buffedCard);
                            // jyoukyou kiin syori
                            auto itr = enemyBoardCardCopy.begin();
                            itr = enemyBoardCardCopy.begin();
                            while (itr != enemyBoardCardCopy.end())
                            {
                                if ((*itr)->getDefense() <= 0)
                                {
                                    itr = enemyBoardCardCopy.erase(itr);
                                }
                                else
                                {
                                    itr++;
                                }
                            }
                        }
                        else{
                            enemyBoardCardCopy.push_back(card);
                        }
                    }
                    Game* current = new Game(myHandCard, myBoardCard, enemyBoardCardCopy, copyMe, copyOpponent, "", manaLeft);
                    while(current->getCommands().find("PASS") == std::string::npos){
                        current = current->simulation(false);
                    }
                    int score = current->getGameBoardScore() - currentBoardScore - 1 + hand->getDraw();
                    if(score > maxScore){
                        maxScore = score;
                        hand->setPlayScore(score);
                        std::stringstream ss;
                        ss << "USE " << hand->getID() << " " << targetCreature->getID() << ";";
                        hand->setCommand(ss.str());
                    }
                }
            }
            else{
                hand->setPlayScore(-999);
            }
        }
        else if(hand->getType() == 3){
            if(hand->getDefense() < 0){
                int maxScore = -99999;
                // target player score
                Player* damagedOpponent = copyOpponent->getPlayerCopy();
                damagedOpponent->takeDamage(-hand->getDefense());
                maxScore = getBoardScore(myBoardCard, enemyBoardCard, copyMe, damagedOpponent) - currentBoardScore - 1 + hand->getDraw();
                hand->setPlayScore(maxScore);
                std::stringstream ss;
                ss << "USE " << hand->getID() << " " << "-1" << ";";
                hand->setCommand(ss.str());

                for(Card* targetCreature : enemyBoardCard){
                    vector<Card*> enemyBoardCardCopy;
                    for(Card* card: enemyBoardCard){
                        if(targetCreature->getID() == card->getID()){
                            Card* buffedCard = card->getCardCopy();
                            buffedCard->deBuff(hand);
                            enemyBoardCardCopy.push_back(buffedCard);
                            // jyoukyou kiin syori
                            auto itr = enemyBoardCardCopy.begin();
                            itr = enemyBoardCardCopy.begin();
                            while (itr != enemyBoardCardCopy.end())
                            {
                                if ((*itr)->getDefense() <= 0)
                                {
                                    itr = enemyBoardCardCopy.erase(itr);
                                }
                                else
                                {
                                    itr++;
                                }
                            }
                        }
                        else{
                            enemyBoardCardCopy.push_back(card);
                        }
                    }
                    Game* current = new Game(myHandCard, myBoardCard, enemyBoardCardCopy, copyMe, copyOpponent, "", manaLeft);
                    while(current->getCommands().find("PASS") == std::string::npos){
                        current = current->simulation(false);
                    }
                    int score = current->getGameBoardScore() - currentBoardScore - 1 + hand->getDraw();
                    if(score > maxScore){
                        maxScore = score;
                        hand->setPlayScore(score);
                        std::stringstream ss;
                        ss << "USE " << hand->getID() << " " << targetCreature->getID() << ";";
                        hand->setCommand(ss.str());
                    }
                }
            }
            else{
                int score = getBoardScore(myBoardCard, enemyBoardCard, copyMe, copyOpponent) - currentBoardScore + 1 + hand->getDraw();
                hand->setPlayScore(score);
                std::stringstream ss;
                ss << "USE " << hand->getID() << " -1" << ";";
                hand->setCommand(ss.str());
            }
        }
        else if(hand->getType() == 0){
            if(hand->isCharge() && myBoardCard.size() < 6){
                vector<Card*> myBoardCardCopy;
                for(Card* card: myBoardCard){
                    myBoardCardCopy.push_back(card);
                }
                myBoardCardCopy.push_back(hand);

                Game* current = new Game(myHandCard, myBoardCardCopy, enemyBoardCard, copyMe, copyOpponent, "", manaLeft);
                while(current->getCommands().find("PASS") == std::string::npos){
                    current = current->simulation(false);
                }
                int score = current->getGameBoardScore() - currentBoardScore - 1 + hand->getDraw();
                hand->setPlayScore(score);
                std::stringstream ss;
                ss << "SUMMON " << hand->getID() << ";";
                hand->setCommand(ss.str());
            }
            else if(myBoardCard.size() < 6){
                vector<Card*> myBoardCardCopy;
                for(Card* card: myBoardCard){
                    myBoardCardCopy.push_back(card);
                }
                myBoardCardCopy.push_back(hand);
                hand->setPlayScore(getBoardScore(myBoardCardCopy, enemyBoardCard, copyMe, copyOpponent) - currentBoardScore);
                std::stringstream ss;
                ss << "SUMMON " << hand->getID() << ";";
                hand->setCommand(ss.str());
            }
            else{
                hand->setPlayScore(-999);
            }
        }
#ifdef DEBUG_PRINT
        cerr << hand->getID() << "playValueBefore=" << hand->getPlayScore() << endl;
#endif
    }

    void playCard(Card* card){
        // remove from hand
        auto itr = myHandCard.begin();
        while (itr != myHandCard.end())
        {
            if ((*itr)->getID() == card->getID())
            {
                itr = myHandCard.erase(itr);
                break;
            }
            else
            {
                itr++;
            }
        }

        // health change
        me->takeDamage(-card->getMyHealthChange());
        opponent->takeDamage(-card->getOpponentHealthChange());

        if(card->getType() == 0 && myBoardCard.size() < 6){
            ss << "SUMMON " << card->getID() << ";";
            addMyCardToBoard(card);
            // charge
            if(!card->isCharge()){
                card->doAction();
            }
            manaLeft -= card->getCost();
        }
        else if(card->getType() == 1 && card->getCommand() != ""){
            ss << card->getCommand();
            int targetID = std::atoi(split(card->getCommand(), ' ')[2].c_str());
            for(Card* target: myBoardCard){
                if(targetID == target->getID()){
                    target->buff(card);
                    break;
                }
            }
            manaLeft -= card->getCost();
        }
        else if(card->getType() == 2 && card->getCommand() != ""){
            ss << card->getCommand();
            int targetID = std::atoi(split(card->getCommand(), ' ')[2].c_str());
            for(Card* target: enemyBoardCard){
                if(targetID == target->getID()){
                    target->deBuff(card);
                    // jyoukyou kiin syori
                    auto itr = enemyBoardCard.begin();
                    itr = enemyBoardCard.begin();
                    while (itr != enemyBoardCard.end())
                    {
                        if ((*itr)->getDefense() <= 0)
                        {
                            itr = enemyBoardCard.erase(itr);
                        }
                        else
                        {
                            itr++;
                        }
                    }
                    break;
                }
            }
            manaLeft -= card->getCost();
        }
        else if(card->getType() == 3 && card->getCommand() != ""){
            ss << card->getCommand();
            int targetID = std::atoi(split(card->getCommand(), ' ')[2].c_str());
            if(targetID != -1){
                for(Card* target: enemyBoardCard){
                    if(targetID == target->getID()){
                        target->deBuff(card);
                        // jyoukyou kiin syori
                        auto itr = enemyBoardCard.begin();
                        itr = enemyBoardCard.begin();
                        while (itr != enemyBoardCard.end())
                        {
                            if ((*itr)->getDefense() <= 0)
                            {
                                itr = enemyBoardCard.erase(itr);
                            }
                            else
                            {
                                itr++;
                            }
                        }
                        break;
                    }
                }
            }
            manaLeft -= card->getCost();
        }
    }

    string getCommands(){
        return ss.str();
    }

    void addMyCardToBoard(Card* card){
        myBoardCard.push_back(card);
    }

    void addString(string str){
       ss << str;
    }
};

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

// mana curve
int* manaCurve = new int[13];
//int idealManaCurve[13] = {2, 6, 4, 6, 3, 3, 2, 2, 1, 1, 1, 1, 1};

// by statistics
int idealManaCurve[13] = {1, 4, 3, 6, 4, 4, 3, 4, 1, 2, 0, 0, 2};

// card cost counts
int catdCostCounts[13] = {7,12,33,28,30,18,15,7,5,3,0,0,2};

int* currentDeck = new int[160];
int pickedCard = 0;
int HIDDEN_SIZE = 100;
double*** w;
double** b;

double ** relu(double** input, int r, int c){
    for(int i = 0; i < r; i ++){
        for(int n = 0; n < c; n ++){
            if(input[i][n] < 0){
                input[i][n] = 0;
            }
        }
    }
    return input;
}

double ** sigmoid(double** input, int r, int c){
    for(int i = 0; i < r; i ++){
        for(int n = 0; n < c; n ++){
            input[i][n] = 1/(1 + exp(-input[i][n]));
        }
    }
    return input;
}

double ** tanh(double** input, int r, int c){
    for(int i = 0; i < r; i ++){
        for(int n = 0; n < c; n ++){
            input[i][n] = 1 - exp(-2 * input[i][n]) / (1 + exp(-2 * input[i][n]));
        }
    }
    return input;
}

string activation;

double ** applyActivation(double** input, int r, int c){
    cerr << activation << endl;
    if(activation == "\"relu\""){
        return relu(input, r, c);
    }
    if(activation == "\"tanh\""){
        return tanh(input, r, c);
    }
    if(activation == "\"sigmoid\""){
        return sigmoid(input, r, c);
    }
    //exit(1);
}

double** getOneHot(){
    double ** result = new double* [640];
    for(int i = 0; i < 640; i++){
        result[i] = new double[1];
        result[i][0] = 0;
    }
    for(int i = 0; i < 160; i++){
        if(currentDeck[i] == 0){
            result[i * 4][0] = 1;
        }
        if(currentDeck[i] == 1){
            result[i * 4 + 1][0] = 1;
        }
        if(currentDeck[i] == 2){
            result[i * 4 + 2][0] = 1;
        }
        if(currentDeck[i] >= 3){
            result[i * 4 + 3][0] = 1;
        }
    }
    return result;
}

double** calcNN(){
    double** inputVector = getOneHot();

    // calc matrix 1
    cerr << "layer1" << endl;
    double** x1 = mulMatrix(w[0], inputVector, 100, 640, 640, 1);
    cerr << "endmul" << endl;
    for(int i = 0; i < HIDDEN_SIZE; i ++){
        x1[i][0] += b[0][i];
    }
    applyActivation(x1, HIDDEN_SIZE, 1);
    // calc matrix 2
    cerr << "layer2" << endl;
    double** x2 = mulMatrix(w[1], x1, 100, 100, 100, 1);
    for(int i = 0; i < HIDDEN_SIZE; i ++){
        x2[i][0] += b[1][i];
    }
    applyActivation(x2, HIDDEN_SIZE, 1);
    // calc matrix 3
    cerr << "layer3" << endl;
    double** x3 = mulMatrix(w[2], x2, 160, 100, 100, 1);
    for(int i = 0; i < 160; i ++){
        x3[i][0] += b[2][i];
    }
    applyActivation(x3, 160, 1);
    return x3;
}

string modelDir;
string logFileName;
string logDirName;
string confDir;

#ifdef READ_EPOCH_FILE
void readModel(){
    std::string str;

    // read activation
    std::ifstream ifs(confDir);
    if (ifs.fail()){
        std::cerr << "失敗" << std::endl;
        return;
    }
    while (getline(ifs, str)){
        if(str.substr(0, 10) == "activation"){
            activation = str.substr(13);
        }
    }

    // load bias
    b = new double*[3];
    for(int n = 0; n < 3; n ++){
        cerr << "read bias file" << n << endl;
        string fileName;
        int size;
        if(n == 0){
            fileName = modelDir + "/Inference-Forward1-biases.csv";
            size = 100;
        } else if(n == 1){
            fileName = modelDir + "/Inference-Forward2-biases.csv";
            size = 100;
        } else if(n == 2){
            fileName = modelDir + "/Inference-Output-biases.csv";
            size = 160;
        }
        std::ifstream ifs(fileName);
        if (ifs.fail()){
            std::cerr << "失敗" << std::endl;
            return;
        }
        b[n] = new double[size];
        int i = 0;
        while (getline(ifs, str)){
            b[n][i] = std::atof(str.c_str());
            i ++;
        }
    }

    // load weight
    w = new double**[3];
    for(int n = 0; n < 3; n ++){
        cerr << "read weight file" << n << endl;
        string fileName;
        int size1, size2;
        if(n == 0){
            fileName = modelDir  + "/Inference-Forward1-weights.csv";
            size1 = 100;
            size2 = 640;
        } else if(n == 1){
            fileName = modelDir  + "/Inference-Forward2-weights.csv";
            size1 = 100;
            size2 = 100;
        } else if(n == 2){
            fileName = modelDir  + "/Inference-Output-weights.csv";
            size1 = 160;
            size2 = 100;
        }
        std::ifstream ifs(fileName);
        if (ifs.fail()){
            std::cerr << "失敗" << std::endl;
            return;
        }
        w[n] = new double*[size1];
        for(int i = 0; i < size1; i ++){
            w[n][i] = new double[size2];
        }
        int i = 0;
        while (getline(ifs, str)){
            vector<string> result = split(str, ' ');
            for(int j = 0; j < size1; j ++){
                w[n][j][i] = std::atof(result[j].c_str());
            }
            i ++;
        }
    }
    cerr << "read weight done" << endl;
}
#endif



int main(int argc,char *argv[])
{

#ifdef DEBUG
    logDirName = argv[1];
#endif

#ifdef READ_EPOCH_FILE
    modelDir = argv[2];
    confDir = argv[3];
    readModel();
    for(int i = 0; i < 160; i ++){
        currentDeck[i] = 1;
    }
    double** nn = calcNN();
    for(int i = 0; i < 160; i ++){
        cerr << nn[i][0] << endl;
    }
#endif

    for(int i = 0; i < 160; i ++){
        currentDeck[i] = 0;
    }
    for(int i = 0; i < 13; i ++){
        manaCurve[i] = 0;
    }
    // game loop
    while (1) {
        vector<Card*> myHandCard;
        vector<Card*> myBoardCard;
        vector<Card*> enemyBoardCard;
        Player* me;
        Player* opponent;
        for (int i = 0; i < 2; i++) {
            int playerHealth;
            int playerMana;
            int playerDeck;
            int playerRune;
            cin >> playerHealth >> playerMana >> playerDeck >> playerRune; cin.ignore();
            if(i == 0){
                me = new Player(playerHealth, playerMana, playerDeck, playerRune);
            }
            else {
                opponent = new Player(playerHealth, playerMana, playerDeck, playerRune);
            }
        }
        int opponentHand;
        cin >> opponentHand; cin.ignore();
        int cardCount;
        cin >> cardCount; cin.ignore();
        for (int i = 0; i < cardCount; i++) {
            int cardNumber;
            int instanceId;
            int location;
            int cardType;
            int cost;
            int attack;
            int defense;
            string abilities;
            int myHealthChange;
            int opponentHealthChange;
            int cardDraw;
            cin >> cardNumber >> instanceId >> location >> cardType >> cost >> attack >> defense >> abilities >> myHealthChange >> opponentHealthChange >> cardDraw; cin.ignore();
            Card* card = new Card(cardNumber, instanceId, location, cardType, cost, attack, defense, abilities, myHealthChange, opponentHealthChange, cardDraw, false);
            if(location == 0){
                myHandCard.push_back(card);
            }
            else if (location == 1){
                myBoardCard.push_back(card);
            }
            else if(location == -1){
                enemyBoardCard.push_back(card);
            }
        }

        std::chrono::system_clock::time_point  start, end; // 型は auto で可
        start = std::chrono::system_clock::now(); // 計測開始時間

        // while draft phase
        Card* lowestCard = nullptr;
        int lowestIndex = -1;
        double maxScore = -9999999;
        int i = 0;
        double expectedManaCurve[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
        for(int n = 0; n < 13; n ++){
            expectedManaCurve[n] = manaCurve[n] + (30 - pickedCard - 1) * ((double)catdCostCounts[n] / 160);
#ifdef DEBUG_PRINT
            cerr << "mana" << n << ":" << manaCurve[n] << endl;
            cerr << "expedtedmana" << n << ":" << expectedManaCurve[n] << endl;
#endif
        }


        // think about mana curve
        if(me->getMana() == 0){
            for(Card* card: myHandCard){
                double score = idealManaCurve[card->getCost()] - expectedManaCurve[card->getCost()];
#ifndef TEKAGEN
                //score += cardValues[card->getCardNumber() - 1] * 200;
                score = cardWinrate[card->getCardNumber() - 1][currentDeck[card->getCardNumber() - 1] + 1] - cardWinrate[card->getCardNumber() - 1][currentDeck[card->getCardNumber() - 1]];
#endif
#ifdef READ_EPOCH_FILE
                score = calcNN()[card->getCardNumber() - 1][0];
#endif
                /*if(card->getType() != 0){
                    score = -9999;
                }*/
                cerr << "cardscore" << card->getCardNumber() << ":" << score << ":" << maxScore << endl;
                if(lowestCard == nullptr || maxScore < score){
                    cerr << score << ":" << maxScore << endl;
                    lowestCard = card;
                    lowestIndex = i;
                    maxScore = score;
                }
                i ++;
            }
#ifdef RANDOM_PICK
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_int_distribution<int> dice(0,10);
            int rnd = dice(mt);
            if(rnd <= 2){
                lowestIndex = rnd;
                lowestCard = myHandCard[lowestIndex];
            }
#endif

            cerr << lowestIndex << endl;
            cout << "PICK " << lowestIndex << endl;

#ifdef DEBUG
            ofstream outputfile(logDirName, ios::app);
            for(int i = 0; i < 160; i++){
                outputfile << currentDeck[i] << " ";
            }
            outputfile << endl;
            for(Card* card: myHandCard){
                outputfile << card->getCardNumber() - 1 << " ";
            }
            outputfile << endl;
            outputfile << lowestCard->getCardNumber() - 1 << endl;
            outputfile.close();
#endif DEBUG

            currentDeck[lowestCard->getCardNumber() - 1]++;
            manaCurve[lowestCard->getCost()]++;
            pickedCard ++;
        }
        else{
            // judge leathal
            bool goToFace = false;
            int totalAttack = 0;
            int totalDefence = 0;
            for(Card* card: myBoardCard){
                totalAttack += card->getAttack();
            }
            for(Card* card: enemyBoardCard){
                totalDefence += card->getDefense();
            }
            if(totalAttack * 2 > opponent->getHealth() + totalDefence){
                goToFace = true;
            }
            
            // battle phase
            cerr << goToFace << endl;
            Game* current = new Game(myHandCard, myBoardCard, enemyBoardCard, me, opponent, "", me->getMana());
            
            // make play card score 
            vector<Card*> beforePlayCards = current->calcBestPlayCards(goToFace);
            for(Card* card: beforePlayCards){
                cerr << "beforePlay:" << card->getCommand() << endl;
                current->playCard(card);
            }

            while(current->getCommands().find("PASS") == std::string::npos){
                current = current->originalSimulation(goToFace);
                cerr << current->getCommands() << endl;
            }

            int passIndex = current->getCommands().find("PASS");
            
            vector<Card*> playCards;

            // play cards untill no more want to play
            playCards = current->calcBestPlayCards(false);
            while(playCards.size() > 0){
                for(Card* card: playCards){
                    current -> playCard(card);
                }
                playCards = current->calcBestPlayCards(false);
            }

            // use chage cards
            current = current->simulation(goToFace);
            while(current->getCommands().find("PASS", passIndex + 1) == std::string::npos){
                current = current->simulation(goToFace);
                cerr << current->getCommands() << endl;
            }

            end = std::chrono::system_clock::now();  // 計測終了時間
            double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count(); //処理に要した時間をミリ秒に変換

            cout << current->getCommands() << "PASS " << elapsed << "ms;" << endl;
        }

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

       
    }
}