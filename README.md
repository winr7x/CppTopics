## How to build

```powershell
$mytarget="10_notify_thread_bad_main"; cmake .. ; cmake --build . --config Release --clean-first --target $mytarget; & .\$mytarget
```

```bash
cmake -S . -B build
cmake --build build -j
```