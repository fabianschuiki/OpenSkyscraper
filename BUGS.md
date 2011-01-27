Linux
=====

Ubuntu 10.10
------------

- For some reason some sounds tend to stop playing at some point, as if OpenAL lost its buffer's content. For example, the morning and evening sounds would still play, but the elevators wouldn't.
*CAUSE:* This problem is likely to be caused by OpenAL running out of sources due to some `SoundEffect` instances not being destructed properly.
- Somehow the transparency doesn't get applied correctly for certain sprites, namely the *city decoration* sprites as well as all *items with windows*, where the sky color is not masked appropriately.

