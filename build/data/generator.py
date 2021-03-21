from random import randint

for i in [500, 1000, 2000, 5000, 10000]:
    for j in [5, 10, 50, 100]:
        edges_count = i // j
        
        with open(f'{i}_{j}.txt', 'w') as f:
            starts = list()
            
            while (len(starts) != 10):
                buf = randint(0, i - 1)
                if (buf not in starts):
                    starts.append(buf)
            
            f.write(f'{i} 10 {edges_count}\n')
            for k in starts:
                f.write(f'{k} ')
            
            f.write('\n')

            edges = list()
            while (len(edges) != edges_count):
                buf_1 = randint(0, len(starts) - 1)
                buf_2 = randint(starts[buf_1], i - 1)
                if ((buf_2 not in starts) and ([starts[buf_1], buf_2] not in edges)):
                    f.write(f'{starts[buf_1]} {buf_2}\n')
                    edges.append([starts[buf_1], buf_2])
                    starts.append(buf_2)
        