#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "solver.h"

typedef struct {
    char puzzle[100];
    char name[20];
} TestCase;

TestCase tests[] = {
    {"SEND + MORE = MONEY", "T1"},
    {"FORTY + TEN + TEN = SIXTY", "T2"},
    {"ELEVEN + NINE + FIVE + FIVE = THIRTY", "T3"},
    {"BE + BE = MOO", "T4"},
    {"QPSS + PQWU + ARGR = GARRR", "T5"},
    {"WJDT + UFTA = LDDWT", "T6"},
    {"PTB + NNN = KPTR", "T7"},
    {"EC + FF = AG", "T8"},
    {"ZX + XZ + CG = SCS", "T9"},
    {"FAH + BAA + FJS = MHPO", "T10"},
    {"WLO + QQO = OZWW", "T11"},
    {"LRAP + WPPP + WLPL = EAPGP", "T12"},
    {"TN + GN = AAL", "T13"},
    {"YCYK + QHWY = KCOYO", "T14"},
    {"DVQQ + UUUN + DJIW = VIWDU", "T15"},
    {"IIOO + YGHO = CITCI", "T16"},
    {"CY + NA = SVQ", "T17"},
    {"LSAL + EJVL = LAAVD", "T18"},
    {"PV + WT = TS", "T19"},
    {"LIIV + GLGV + VVZI + EXGL = XLDXI", "T20"},
    {"PB + DF + RG = FDR", "T21"},
    {"VVQV + GODY + DDDO + TLYD = DDQLG", "T22"},
    {"JJEEO + FFDNG = EDBEX", "T23"},
    {"DVB + VPN = YDB", "T24"},
    {"THTZ + CKKK = ZRKCG", "T25"},
    {"B + P = VT", "T26"},
    {"FPQ + VNC + TNE = NVGN", "T27"},
    {"OO + ZY + QR = QQL", "T28"},
    {"OGWK + JDOK = DJJW", "T29"},
    {"OTOX + ONOF = KXOXC", "T30"},
    {"WBD + KIW + IXW = DKBN", "T31"},
    {"O + X = B", "T32"},
    {"OQCQ + RICT = ETVRO", "T33"},
    {"ALEE + GNLX = IGYAG", "T34"},
    {"AUR + PPR + HHP = UDNX", "T35"},
    {"CMB + GAC + MEA = ALLB", "T36"},
    {"MVX + VAW + XKM = XOXA", "T37"},
    {"MFQ + XIS + QIR = FRGM", "T38"},
    {"LL + YW = FXX", "T39"},
    {"GQXH + QXKX = SIGLG", "T40"},
    {"AAGV + PJRG = RPVPQ", "T41"},
    {"JGJA + SYZP = YIDDJ", "T42"},
    {"KWLS + EFSS = QZMMA", "T43"},
    {"GKAD + HGMD = MOAK", "T44"},
    {"IH + OW = POV", "T45"},
    {"SY + UW = AYG", "T46"},
    {"YYIV + KHYQ + QYNM + RHKR = YKMKY", "T47"},
    {"RDO + DDO + UGO = CGIR", "T48"},
    {"KNKN + RKCK + TITV + IRTU = GTGUU", "T49"},
    {"UXBN + SNTA = BGBCN", "T50"},
    {"LROW + RTVR + ZJOJ = FJVMO", "T51"},
    {"IDDTD + DCULK = KTGTC", "T52"},
    {"MZK + SKQ = ZUZE", "T53"},
    {"MY + FF + DL = DUD", "T54"},
    {"VVAT + UQQT + NTSQ = UUNCV", "T55"},
    {"DAVJ + YXAF = WDHTD", "T56"},
    {"OWJ + JLJ = BOZO", "T57"},
    {"TIHJX + JUHZO = UXIOH", "T58"},
    {"LWV + MFV = ZLZV", "T59"},
    {"ODGU + QGEA + AGUE = QDDQX", "T60"},
    {"XKSIG + XYSSG = TGSIA", "T61"},
    {"MOOA + AAAK + WUSL = AUMSU", "T62"},
    {"WYII + TWYY = PWBXP", "T63"},
    {"YYU + OEQ = IZQP", "T64"},
    {"HMM + ZOK + KIM = HZKX", "T65"},
    {"LWXW + EXUT + ENXM = WNUXL", "T66"},
    {"JQA + JAM + OAQ = XVVX", "T67"},
    {"XNA + OXR = LPQA", "T68"},
    {"ZMMV + ILXM = TQLXV", "T69"},
    {"QAPWK + GGEQW = SAKPKW", "T70"},
    {"BLWB + ZZXB = WXZNH", "T71"},
    {"PPKT + ERTG = RDGKG", "T72"},
    {"YY + YY = OO", "T73"},
    {"VVKY + AVIY + SSVB = TBSTB", "T74"},
    {"RQAH + ZHQY = AFYIJ", "T75"},
    {"IRRK + KRVK + UKEU = IIBKU", "T76"},
    {"LH + GK = AK", "T77"},
    {"YSIS + OYTF = LLTNS", "T78"},
    {"UTA + UDY = QRV", "T79"},
    {"FHF + MVM = PHP", "T80"},
    {"V + V = IN", "T81"},
    {"WVQ + ADQ = DBBW", "T82"},
    {"IWJ + VGW = GGGJ", "T83"},
    {"IVAIR + EGAIX = ZVZXE", "T84"},
    {"GP + UQ = VQ", "T85"},
    {"SJHD + SJLB = QBAY", "T86"},
    {"OPBF + PEQU = FFBUB", "T87"},
    {"XSWH + WHIB = PKWWT", "T88"},
    {"BZRZ + XXBS + CDNS = XDXRZ", "T89"},
    {"QQVVH + VHBIG = AVSNMH", "T90"},
    {"EXECA + SAEAR = RDLEV", "T91"},
    {"BPBVJ + BCVOJ = YOVCBP", "T92"},
    {"KTH + QHT + TMV = FFVH", "T93"},
    {"QMH + EGQ = HQHV", "T94"},
    {"KUQ + UJJ + KHU = TVRS", "T95"},
    {"JJYI + GVJI + GJLL = IYOGY", "T96"},
    {"YLJF + VYYJ = HGQRE", "T97"},
    {"EPUQ + PEFP = LPUPL", "T98"},
    {"SSZG + HOIB + IGHA = PAHZC", "T99"},
    {"AB + NC + PO = NYP", "T100"},
    {"YN + VF = PN", "T101"},
    {"CI + TE = BMN", "T102"},
    {"X + E = DR", "T103"},
    {"TFLY + LHUT + FGHU = TTNGN", "T104"},
    {"TYUVFRHGJHYTYV + ABCABAC = PJEECKHQR", "T105"},
};

int main() {
    printf("=== Cryptarithmetic Solver - V0 ===\n");
    printf("========================================\n\n");
    
    for (int i = 0; i < 105; i++) {
        printf("Test %s: %s\n", tests[i].name, tests[i].puzzle);
        
        char result[1000] = {0};
        
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        
        int found = solve_rebus(tests[i].puzzle, result);
        
        clock_gettime(CLOCK_MONOTONIC, &end);
        double elapsed = (end.tv_sec - start.tv_sec) + 
                        (end.tv_nsec - start.tv_nsec) / 1e9;
        
        if (found) {
            printf("Solution: %s\n", result);
        } else {
            printf("No solution found\n");
        }
        printf("Time: %.6f sec\n\n", elapsed);
    }
    
    return 0;
}
