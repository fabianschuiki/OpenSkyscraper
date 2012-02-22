This is the experimental `restart` branch of OpenSkyscraper.

**You are welcome to join in the experiments, put forward suggestions or supply your own experimental code!**


What is being tested?
---------------------
Currently we're performing the following experiments and tests:

- **SFML** as platform API (instead of SDL)
  - loads bitmaps, fonts and sounds directly from memory
  - modern C++ interface simplifies code

- **Lua** as scripting language for
  - game logic
  - animation
  - drawing

  
Why such heavy scripting?
-------------------------
Writing code in a scripting language is generally faster and more convenient than in C++, as rigorous typesafety and non-introspectivity usually gets in the your way in the latter. Some may argue that writing time-critical code in a scripting language is a bad idea. Yet modern game engines usually encourage the AI to be written in a scripting language. Furthermore, Lua is pretty fast (when used at a low level, e.g. through ObjectiveLua) and there are games that are written entirely in Lua (CorsixTH).


Coding Style
------------

- Everything resides in the OT namespace.
- Source file names match the class names in capitalization, e.g. the class `Application` yields `Application.{h|cpp}`.
- The directory hierarchy reflects the namespace hierarchy, so `Math::Vector2D` goes into `Math/Vector2D.{h|cpp}`.

There are some violations of this style left in the source. I'm working on getting rid of them.

*General note on source files:* Lowercase source files are likely to be outdated and removed in the future. They're probably there because they contain some working code snippet.
