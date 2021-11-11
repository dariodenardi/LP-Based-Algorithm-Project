# LP-Based Algorithm

Implementation of a Heuristic Algorithm for GMKP problem with CPLEX APIs in C++

## Requirements

* First, you must generate instaces with my other program. See my [project](https://github.com/dariodenardi/GMKP-Project).

## Programs

You can find releases at the top right.

## Code compilation

* IDE C++ (e.g. Visual Studio)

### Configuration Visual Studio 2017 for CPLEX Libraries

Right click on project -- Property

C/C++ -- Additional include directories -- General:
```
C:\Program Files\IBM\ILOG\CPLEX_Studio129\cplex\include
C:\Program Files\IBM\ILOG\CPLEX_Studio129\concert\include
```

C/C++ -- Preprocessor -- Preprocessor definitions:
```
WIN32
_CONSOLE
IL_STD
_CRT_SECURE_NO_WARNINGS
```

Linker -- Input -- Additional dependencies:
```
C:\Program Files\IBM\ILOG\CPLEX_Studio129\cplex\lib\x64_windows_vs2017\stat_mda\cplex1290.lib
C:\Program Files\IBM\ILOG\CPLEX_Studio129\cplex\lib\x64_windows_vs2017\stat_mda\ilocplex.lib
C:\Program Files\IBM\ILOG\CPLEX_Studio129\concert\lib\x64_windows_vs2017\stat_mda\concert.lib
```

C/C++ -- Code Generation -- For 64 bit windows: code generation:
```
Multi-threaded DLL (/MD)
```

NOTE: version of CPLEX used is 12.9. In later versions the folders can change

## License

The source code for the site is licensed under the GNU General Public License v3, which you can find in the LICENSE.md file.
