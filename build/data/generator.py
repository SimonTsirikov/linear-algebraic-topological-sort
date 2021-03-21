from random import randint

for i in [500, 1000, 2000, 5000, 10000]:
    edges_count = 0
    with open(f'{i}.txt', 'w') as f:
        starts = set()
        
        for k in range(10):
            starts.add(randint(0, i - 1))
        
        f.write('\n\n')
        for k in starts:
            f.write(f'{k} ')
        
        f.write('\n')

        for k in range(i):
            for l in range(k, i):
                if ((k not in starts) and (randint(0, 1) == 1)):
                    f.write(f'{k} {l}\n')
                    edges_count += 1

    with open(f'{i}.txt', 'r') as f:
        lines = f.readlines()

    lines[0] = f'{i} {edges_count}'

    with open(f'{i}.txt', 'w') as f:
        f.writelines(lines)
