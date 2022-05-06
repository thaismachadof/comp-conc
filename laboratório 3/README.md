## Ganho de desempenho

Considerando a aceleração como Tempo<sub>sequencial</sub> / Tempo<sub>concorrente</sub>, temos:

|    Dimensão    | N<sup>o</sup> de threads  | Tempo sequencial | Tempo concorrente | Ganho real |
|----------------|---------------------------|------------------|-------------------|------------|
|10<sup>5</sup>  |      1     |0.000259      | 0.000622 | 0.416398
|10<sup>5</sup>  |      2     |0.000259      | 0.000523 | 0.495219
|10<sup>5</sup>  |      4     |0.000260      | 0.000665 | 0.390977
|10<sup>7</sup>  |      1     |0.025724      | 0.029122 | 0.883318
|10<sup>7</sup>  |      2     |0.025218      | 0.014910 | 1.691348
|10<sup>7</sup>  |      4     |0.025498      | 0.007999 | 3.187648
|10<sup>8</sup>  |      1     |0.255001      | 0.279816 | 0.911316
|10<sup>8</sup>  |      2     |0.254984      | 0.142937 | 1.783890
|10<sup>8</sup>  |      4     |0.142937      | 0.077163 | 3.301971
