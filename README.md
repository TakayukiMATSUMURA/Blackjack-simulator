# Blackjack-simulator
Blackjack simulator, which simulates a player with [basic strategy](http://www.e-town21.com/lasvegas/casino/blackjack/bj_2.htm),
[card counting](https://en.wikipedia.org/wiki/Card_counting) and calculates expected value.

## Example
    ./blackjack-simulator -g 10000
    Blackjack simulator
    Rule
    deck:6
    penetration:75%

    Config
    strategy:basic
    game:10000

    Simulation start
    Simulation finish

    Simulation results
    ############# Player ##############
    Result
    [1] lose            5079 ( 49.575%)
    [2] win             4362 ( 42.577%)
    [3] tie              804 (  7.848%)
    [-] total          10245 (100.000%)

    Insurance accuracy
    NaN
    Surrender
    4.829147%(448/9277)

    Double down
    [1] win              574 ( 53.947%)
    [2] lose             417 ( 39.192%)
    [3] tie               73 (  6.861%)
    [-] total           1064 (100.000%)

    Split
    [1] 1                177 ( 84.689%)
    [2] 2                 28 ( 13.397%)
    [3] 3                  4 (  1.914%)
    [-] total            209 (100.000%)

    Hand distribution
    [ 1] 20              1471 ( 14.358%)
    [ 2] Bust            1360 ( 13.275%)
    [ 3] 17              1147 ( 11.196%)
    [ 4] 19              1116 ( 10.893%)
    [ 5] 18              1056 ( 10.307%)
    [ 6] 15               679 (  6.628%)
    [ 7] 16               664 (  6.481%)
    [ 8] 21               523 (  5.105%)
    [ 9] 13               507 (  4.949%)
    [10] BJ               493 (  4.812%)
    [11] 14               435 (  4.246%)
    [12] Dealer BJ        431 (  4.207%)
    [13] 12               357 (  3.485%)
    [14] 11                 6 (  0.059%)
    [--] total          10245 (100.000%)

    Expected value
    -0.881016%


    ############# Dealer ##############
    Hand distribution
    [1] Bust            2865 ( 28.650%)
    [2] 20              1848 ( 18.480%)
    [3] 17              1449 ( 14.490%)
    [4] 18              1416 ( 14.160%)
    [5] 19              1262 ( 12.620%)
    [6] 21               712 (  7.120%)
    [7] BJ               448 (  4.480%)
    [-] total          10000 (100.000%)

## Reference
[カードカウンティング入門](https://www.amazon.co.jp/%E3%82%AB%E3%83%BC%E3%83%89%E3%82%AB%E3%82%A6%E3%83%B3%E3%83%86%E3%82%A3%E3%83%B3%E3%82%B0%E5%85%A5%E9%96%80-%E3%82%AB%E3%82%B8%E3%83%8E%E3%83%96%E3%83%83%E3%82%AF%E3%82%B7%E3%83%AA%E3%83%BC%E3%82%BA-%E3%82%AA%E3%83%A9%E3%83%95-%E3%83%B4%E3%82%A1%E3%83%B3%E3%82%AF%E3%83%A9%E5%8D%9A%E5%A3%AB/dp/477594908X/ref=sr_1_fkmr0_1?ie=UTF8&qid=1504794337&sr=8-1-fkmr0&keywords=KO+%E3%82%AB%E3%82%A6%E3%83%B3%E3%83%86%E3%82%A3%E3%83%B3%E3%82%B0)
