## Polecenia
### kompilacja i uruchomienie
```sh
make && ./bin/server
```

### perf tests
```sh
time ./scripts/parallel.sh -vs 100
```
or
```sh
httperf --hog --server localhost --port 8080 --num-conns=1000 --rate 100000
```

### czyszczenie folderu obj
```sh
make clean
```

### czyszczenie folderu obj i bin
```
make remove
```