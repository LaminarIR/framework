#ifndef __FUSION_H
#define __FUSION_H

#define max(A,B) (((A)>(B))?(A):(B))
#define pow2ceil(A) ((A<=256)?(256):(((A<=1024)?(1024):(((A<=4096)?(4096):(((A<=16384)?(16384):(((A<=65536)?(65536):(((A<=131072)?(131072):(((A<=262144)?(262144):(((A<=524288)?(524288):(((A<=1048576)?(1048576):(((A<=2097152)?(2097152):(((A<=4194304)?(4194304):(((A<=8388608)?(8388608):(((A<=16777216)?(16777216):(((A<=33554432)?(33554432):(((A<=67108864)?(67108864):(((A<=134217728)?(134217728):(((A<=268435456)?(268435456):(((A<=536870912)?(536870912):(1073741824))))))))))))))))))))))))))))))))))))
#define __BUF_SIZE_MASK_0_1 (pow2ceil(4+0)-1)

#define __BUF_SIZE_MASK_1_2 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_2_3 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_3_4 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_4_5 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_4_266 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_5_6 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_6_7 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_7_8 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_8_9 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_9_10 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_10_11 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_11_12 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_11_265 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_12_13 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_13_14 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_14_15 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_15_16 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_16_17 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_17_18 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_18_19 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_18_264 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_19_20 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_20_21 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_21_22 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_22_23 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_23_24 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_24_25 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_25_26 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_25_263 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_26_27 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_27_28 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_28_29 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_29_30 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_30_31 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_31_32 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_32_33 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_32_262 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_33_34 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_34_35 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_35_36 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_36_37 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_37_38 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_38_39 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_39_40 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_39_261 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_40_41 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_41_42 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_42_43 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_43_44 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_44_45 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_45_46 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_46_47 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_46_260 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_47_48 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_48_49 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_49_50 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_50_51 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_51_52 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_52_53 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_53_54 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_53_259 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_54_55 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_55_56 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_56_57 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_57_58 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_58_59 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_59_60 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_60_61 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_60_258 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_61_62 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_62_63 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_63_64 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_64_65 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_65_66 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_66_67 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_67_68 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_67_257 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_68_69 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_69_70 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_70_71 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_71_72 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_72_73 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_73_74 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_74_75 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_74_256 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_75_76 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_76_77 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_77_78 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_78_79 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_79_80 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_80_81 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_81_82 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_81_255 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_82_83 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_83_84 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_84_85 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_85_86 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_86_87 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_87_88 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_88_89 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_88_254 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_89_90 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_90_91 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_91_92 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_92_93 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_93_94 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_94_95 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_95_96 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_95_253 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_96_97 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_97_98 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_98_99 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_99_100 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_100_101 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_101_102 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_102_103 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_102_252 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_103_104 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_104_105 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_105_106 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_106_107 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_107_108 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_108_109 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_109_110 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_109_251 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_110_111 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_111_112 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_112_113 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_113_114 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_114_115 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_115_116 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_116_117 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_116_250 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_117_118 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_118_119 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_119_120 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_120_121 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_121_122 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_122_123 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_123_124 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_123_249 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_124_125 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_125_126 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_126_127 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_127_128 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_128_129 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_129_130 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_130_131 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_130_248 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_131_132 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_132_133 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_133_134 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_134_135 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_135_136 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_136_137 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_137_138 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_137_247 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_138_139 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_139_140 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_140_141 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_141_142 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_142_143 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_143_144 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_144_145 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_144_246 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_145_146 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_146_147 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_147_148 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_148_149 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_149_150 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_150_151 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_151_152 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_151_245 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_152_153 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_153_154 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_154_155 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_155_156 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_156_157 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_157_158 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_158_159 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_158_244 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_159_160 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_160_161 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_161_162 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_162_163 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_163_164 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_164_165 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_165_166 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_165_243 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_166_167 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_167_168 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_168_169 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_169_170 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_170_171 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_171_172 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_172_173 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_172_242 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_173_174 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_174_175 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_175_176 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_176_177 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_177_178 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_178_179 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_179_180 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_179_241 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_180_181 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_181_182 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_182_183 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_183_184 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_184_185 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_185_186 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_186_187 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_186_240 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_187_188 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_188_189 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_189_190 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_190_191 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_191_192 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_192_193 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_193_194 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_193_239 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_194_195 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_195_196 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_196_197 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_197_198 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_198_199 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_199_200 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_200_201 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_200_238 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_201_202 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_202_203 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_203_204 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_204_205 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_205_206 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_206_207 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_207_208 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_207_237 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_208_209 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_209_210 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_210_211 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_211_212 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_212_213 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_213_214 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_214_215 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_214_236 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_215_216 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_216_217 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_217_218 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_218_219 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_219_220 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_220_221 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_221_222 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_221_235 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_222_223 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_223_224 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_224_225 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_225_226 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_226_227 (pow2ceil(129+0)-1)

#define __BUF_SIZE_MASK_227_228 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_227_234 (pow2ceil(1+0)-1)

#define __BUF_SIZE_MASK_228_229 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_229_230 (pow2ceil(256+0)-1)

#define __BUF_SIZE_MASK_230_231 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_231_232 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_232_233 (pow2ceil(32+0)-1)

#define __BUF_SIZE_MASK_234_229 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_235_223 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_236_216 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_237_209 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_238_202 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_239_195 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_240_188 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_241_181 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_242_174 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_243_167 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_244_160 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_245_153 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_246_146 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_247_139 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_248_132 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_249_125 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_250_118 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_251_111 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_252_104 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_253_97 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_254_90 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_255_83 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_256_76 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_257_69 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_258_62 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_259_55 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_260_48 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_261_41 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_262_34 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_263_27 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_264_20 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_265_13 (pow2ceil(128+0)-1)

#define __BUF_SIZE_MASK_266_6 (pow2ceil(128+0)-1)

#endif
