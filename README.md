# Video Highlight

<iframe width="640" height="360" src="https://www.youtube.com/embed/cFZIgq3tIok" frameborder="0" allowfullscreen></iframe>
<br />
<hr>

# Prominent Features
## Attack Circle
The game has a different approach to handling combat and enemies attacking. I concocted my own version of Kingdoms of Amalur’s Belgian AI system which uses a grid centered on the player to assign open slots to enemies.

The system, which I have called attack circle, limits the number of enemies that can be present in the circle at one time by having a maximum enemy score. Each enemy is assigned a score based on their strength and the combined total of the scores for each enemy in the circle can never exceed the maximum amount allowed. To first join the circle, the circle checks if there is enough available score to accommodate the enemy, and if the enemy can join, it adds the enemy to the circle and removes the enemy’s score from the amount available. Once an enemy inside the circle dies, its score is then added back to the circle’s available amount.

The circle also controls the amount of attacks that can be occurring simultaneously. When some enemy wishes to attack, it requests permission from the attack circle which checks if the score of the requested attack is less than or equal to the available attack score. This allows enemies to make multiple attacks at once, but keep attacks within a controllable amount and strength. This would keep two enemies from each using what would be considered their ultimate move at the same time if the attacks score would exceed the maximum or available attack score.

<br />
<hr>

## Grid-Based Attack Directions
All attacks in the game are based on the direction of a 3x3 grid resembling a tic-tac-toe board. A good way to think of choosing the direction is to either imagine the sword follows the movement of your mouse, or that your mouse is in the center square of the grid and to make attacks other than a thrust, you must move your mouse to be in the correct square. An example of the directions in the grid is shown below:

![Attack Circle](https://i.imgur.com/1H11Dfi.png)

The attack direction grid is then combined with three separate types of attacks: quick, strong, and feint. The three attacks all utilize the attack directions to make for 27 unique attacks that a player can choose. Another gameplay system using the attack direction grid is the parry system which allows the player to choose the direction they wish to parry attacks. Therefore, a parry with the mouse moving upward will result in an overhead parry by the character.

<br />
<hr>

## N-Gram Attack Predictions
Cyberpunk’d also incorporates a machine learning based technique of using N-grams using past attacks to make predictions about what the next attack from the player will be by analyzing patterns of attacks previously seen and recorded. While this allows for the AI of the game to make better informed choices, it doesn’t allow them to instantly react to an attack by “cheating” to get the exact attack direction and type the player has just started. The goal is to make the player change up fighting styles as the game progresses. This is accomplished in large part by weighting recent trends more heavily than the overall history of the player’s attacks. The N-gram system also attempts to provide better results by modifying itself based on whether its predictions are correct or false.

```
This is attempting to predict the next attack of the player given the previous two attacks made.

Attack1 is previous attack used by the player;
Attack2 is the current attack made by the player;
int TotalAttacks;

for(int i = 0; i < array length; i++){
    Attack = 3DArray[Attack1][Attack2][i];
    create map entry(Attack ID, Attack total);
    TotalAttacks += Attack total;
    }
foreach Attack in deque History{
    if Attack1 and Attack2 match the history{
        TotalAttacks -= map[Attack];
        map[Attack] *= WeightForHistory;
        TotalAttacks += map[Attack];
        }
    }
    
int runningTotal;
float RandomFloat;
foreach Attack in map{
    if(RandomFloat < map[Attack] + runningTotal / TotalAttacks)
        return Attack;
    else
        runningTotal += map[Attack];
}
```

The returned attack can be saved as a variable and checked on the player's next attack to see if the prediction was correct and a Condfidence float percentage can be adjusted accordingly. I recommend small changes to the confidence while being clamped to a max and min value so just getting a small series of correct or incorrect predictions doesn't skew how well the algorithm is truly working.

<br />
<hr>

# Code Repository
## [Dragoon Repo](https://github.com/scuhooper/Dragoon/)
This repo runs on 3 core branches. The Gold branch is a stable version of the game with features fully working. The WeeklyBuild branch is just an update from the InDevelopment branch done every Sunday night/Monday morning, and the InDevelopment branch is the branch used for daily development changes and may not be in a stable, playable condition at all times.
