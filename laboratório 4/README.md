## Ganho de desempenho

Considerando a aceleração como Tempo<sub>sequencial</sub> / Tempo<sub>concorrente</sub>, temos:

|    Dimensão    | N<sup>o</sup> de threads  | Tempo sequencial | Tempo concorrente | Ganho real |
|----------------|---------------------------|------------------|-------------------|------------|
|10<sup>5</sup>  |      1                    |0.034473          | 0.034500          | 0.999217
|10<sup>5</sup>  |      2                    |0.033502          | 0.020260          | 1.653603
|10<sup>5</sup>  |      4                    |0.033948          | 0.014886          | 2.280532
|10<sup>7</sup>  |      1                    |3.293847          | 3.374085          | 0.976219
|10<sup>7</sup>  |      2                    |3.271783          | 1.948871          | 1.678809
|10<sup>7</sup>  |      4                    |3.262167          | 1.380706          | 2.362680
|10<sup>8</sup>  |      1                    |34.94813          | 35.40800          | 0.987012
|10<sup>8</sup>  |      2                    |33.16871          | 20.13357          | 1.647433
|10<sup>8</sup>  |      4                    |33.94924          | 14.06221          | 2.414217
