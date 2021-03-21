from random import randint

for i in [500, 1000, 2000, 5000, 10000]:
    for j in [1, 10, 20, 50, 100]:
        edges_count = 0
        with open(f'{i}_{j}.txt', 'w') as f:
            starts = set()
            
            for k in range(j):
                starts.add(randint(0, i - 1))
            
            f.write('\n\n')
            for k in range(j):
                f.write(f'{k} ')
            
            f.write('\n')

            for k in range(i):
                for l in range(max(j, k), i):
                    if (randint(0, 1) == 1):
                        f.write(f'{k} {l}\n')
                        edges_count += 1

        with open(f'{i}_{j}.txt', 'r') as f:
            lines = f.readlines()

        lines[0] = f'{i} {j} {edges_count}'

        with open(f'{i}_{j}.txt', 'w') as f:
            f.writelines(lines)
