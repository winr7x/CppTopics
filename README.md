## How to build

```powershell
$mytarget="10_notify_thread_bad_main"; cmake .. ; cmake --build . --config Release --clean-first --target $mytarget; & .\$mytarget
```

## Src File Template
```
* The ESSENCE of src file
    This essence is already presented in src file name
    Use this place to express essense in whole sentence instead of
    short file name.

* Comparison with the PREVIOUS src file and NEXT src file
    Perhaps this src file is the 3rd way to declare variable
    or good example of code after bad example in previous src file.
    Point such relation in this place

* List the NON-OBVIOUS THESES
    Describe moments which you didn't understand or understood vaguely

* Leave REFERENCE MARKS in theses like '(see ZZ9)' and repeat that marks in code in comments
```