This project contains code from the COMP2501 demos: 
Copyright (c) 2020-2023 Oliver van Kaick <Oliver.vanKaick@carleton.ca>, David Mould <mould@scs.carleton.ca>
under the MIT License.

"Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), 
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."

Created by: Diego Nino & Zhi Ou-Yang 

How to play:
shoot enemies, collect powerups to survive and don't die

Controls:
W - Moving forward
S - Moving backwards
A - Turning left
D - Turnin right

Space - Firing standard gun 
E - Fires shotgun

--Assignment Requirements--

Game mechanics:
	- simple condition of surviving as long as you can

Enemies:
	- 4 different enemies with unique behaviours and textures
	- Enemy 1 charges straight at you
	- Enemy 2 chases you, fires when close and retreats when approached
	- Enemy 3 is smaller and faster and moves in unique manner
	- Enemy 4 is a turret
	- they spawn randomly over time

Weapons:
	- 2 weapons exist in the game, a regular gun and a shotgun
	- i shouldn't have to go into detail on what the difference is

Collectible items:
	- 3 items exist in the game: a invincibility item, a damage up item and a hp up item
	- the invincibility item is a timed item
	- they spawn randomly over time too

Movement and Transformations:
	- not really sure how to describe it but the player and enemies move using parametric based equations
	- also uses translastions and rotations

Collision detection:
	- bullets use a ray-circle collision
	- everything else uses circle-circle collision

Game World:
	- game world is big and can scroll to access other areas

Particle systems:
	- particles are used for 2 effects, one for the player boost and one for the explosion when something dies

UI:
	- there are 4 things shown on the hud of the game: player hp, time, score and the last weapon you used

Advanced method:
	- steering behaviours were used for enemy 2 along with some advanced ai controlling enemies as well

Bonus Points:
	- none i believe

Code readability and organization:
	- i added a lot of comments

Extra:
- project proposal was too ambitious