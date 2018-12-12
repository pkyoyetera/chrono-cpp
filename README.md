
## Chrono-cpp
A C++ library for natural language relative date parsing based on chrono.js

This library is designed with rules to enable extracting absolute date information from text.

So far, only supports phrases such as "(this) (afternoon|evening|night...)".
Features/phrases supported by chrono.js are still under development. Watch the space 


### Building  

##### NOTE: 
Need to have bazel installed.

```bazel build main```

### Running

```./main <phrase with relative time information> <reference date (optional)>```

If the reference date is not provided, the program wil default to the current day.


[wip]
