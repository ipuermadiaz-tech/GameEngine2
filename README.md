*Title*

The game's title is Decently Cool Shoot them up, as a reference to the baseline(spacegame) being jokingly named "super cool
space game" before.


---
*Brief description*

Like the name suggest, the game is a shoot them up, particularly this one inspired by touhou(altough not reaching the complexity
of that at all of course).Its a proof of concept .The engine is really malleable, and handles key game loop operations including actor component management, particle systems, 
random generation utilities, and audio playback.

---

Core features of the game:
-A player character that adheres to the classic shoot them up,being controlled omnidirectionally but always aiming up.
You can use shift to double your speed(unlike the usual halving).Unlike most of the genre, the player does wrap to the sides.
-Generation of enemies through the playthrough and varying spawn conditions.
Diverse behavior that compounds in an interesting if not hard challenge.
-A sliding background made due to making it an enemy constantly spawned, who is in a lower layer(scene drawing had to be 
changed in order for this to not overcome particles),giving a sensation that things are moving up despite being in a static room.
-Altough most of the new elements are derived from the enemy class, there was created an EnemyBullet class, who even has a variant
that is slightly different.Its purpose its to go towards the current position of the player upon creation, and do not interact with player 
bullets.
-The objective of the game is reaching 8000 points,at which point you have a fight and if you win you won the game.


---
The project uses C++17 or higher

*Libraries:*
  * [SDL2](https://www.libsdl.org/) / [SDL3](https://www.libsdl.org/)
  * SDL_mixer (for audio handling)
  * SDL_ttf (for text rendering)
  * Build System:** Visual Studio 2022 / CMake

  Additional credits to m-art for the ost used

