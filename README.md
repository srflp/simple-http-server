## Kompilacja i uruchomienie
### server
```sh
make && ./bin/server
```

### client / perf tests
```sh
time ./parallel.sh -vs 100
```
or
```sh
httperf --hog --server localhost --port 8080 --num-conns=1000 --rate 100000
```