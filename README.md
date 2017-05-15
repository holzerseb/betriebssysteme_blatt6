Task1:
In my tests the computed result was always the expected value.
Though, while it was true in all my tests, the computed value can infact be wrong!
Because all processes access the shared-memory at the same time, one process could eventually fetch an "old" value (because another process hasn't written the new value into the segment), the one process will increment it (to the same value as the other one) and write it - after this, the other process writes the same value. You probably see the issue here.

In fact, I really just increment the shared-memory-segment, so depending on how fast the kernel can perform this ++-increment, there will not be any "overlaps" regarding the previous mentioned issue.


