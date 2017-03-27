# Prominent Features
## Attack Circle
The game has a different approach to handling combat and enemies attacking. I concocted my own version of Kingdoms of Amalur’s Belgian AI system which uses a grid centered on the player to assign open slots to enemies.

The system, which I have called attack circle, limits the amount of enemies that can be present in the circle at one time by having a maximum enemy score. Each enemy is assigned a score based on their strength and the combined total of the scores for each enemy in the circle can never exceed the maximum amount allowed. To first join the circle, the circle checks if there is enough available score to accommodate the enemy, and if the enemy can join, it adds the enemy to the circle and removes the enemy’s score from the amount available. Once an enemy inside the circle dies, its score is then added back to the circle’s available amount.

The circle also controls the amount of attacks that can be occurring simultaneously. When an enemy wishes to attack, it requests permission from the attack circle which checks if the score of the requested attack is less than or equal to the available attack score. This allows enemies to make multiple attacks at once, but keep attacks within a controllable amount and strength. This would keep two enemies from each using what would be considered their ultimate move at the same time if the attacks score would exceed the maximum or available attack score.

<br />
<hr>

## Grid-Based Attack Directions
All attacks in the game are based on the direction of a 3x3 grid resembling a tic-tac-toe board. A good way to think of choosing the direction is to either imagine the sword follows the movement of your mouse, or that your mouse is in the center square of the grid and to make attacks other than a thrust, you must move your mouse to be in the correct square. An example of the directions in the grid is shown below:

Need to create image for the attack grid

The attack direction grid is then combined with three separate types of attacks: quick, strong, and feint. The three attacks all utilize the attack directions to make for 27 unique attacks that a player can choose. Another gameplay system using the attack direction grid is the parry system which allows the player to choose the direction they wish to parry attacks. Therefore, a parry with the mouse moving upward will result in an overhead parry by the character.

<br />
<hr>

## N-Gram Attack Predictions
Cyberpunk’d also incorporates a machine learning based technique of using N-grams using past attacks to make predictions about what the next attack from the player will be by analyzing patterns of attacks previously seen and recorded. While this allows for the AI of the game to make better informed choices, it doesn’t allow them to instantly react to an attack by “cheating” to get the exact attack direction and type the player has just started. The goal is to make the player change up fighting styles as the game progresses. This is accomplished in large part by weighting recent trends more heavily than the overall history of the player’s attacks. The N-gram system also attempts to provide better results by modifying itself based on whether its predictions are correct or false.

`Insert pseudocode for algorithm
and history weighting`

<br />
<hr>

# Code Repository
## [Dragoon Repo](https://github.com/scuhooper/Dragoon)
