# BoxPusher solved using Reinforcement Learning
(LeetCode challenge 1263)

RL solution in "RLutils.h"
* Simulated environment: RL::RLField
* Policy class: RL::EpsGreedyPolilcy
* Q-learning algo: RL::RL_solve

Demo:
* Solution using a (-1) reward for each step and (+100) for reaching the target \\
![boxPusher_demo](run1.gif)
* Solution using a (-1) reward for each step, (-5) for each box move and (+100) for reaching the target \\
![boxPusher_demo](run2.gif)
