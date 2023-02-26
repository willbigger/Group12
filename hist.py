"""
Takes a txt file of advertisers and elements and 
displays two histograms of the frequency from each advertsier

Resources: 
https://realpython.com/python-histograms/
https://stackoverflow.com/questions/21195179/plot-a-histogram-from-a-dictionary
https://realpython.com/sort-python-dictionary/

"""

import matplotlib.pyplot as plt

addrs = []
hist_el = {}
hist_type = {}

with open("test.txt") as f:
    for line in f.readlines():
        line = line.rstrip()
        ln_lst = line.split(" ")
        hist_el[ln_lst[0]] = hist_el.get(ln_lst[0], 0) + 1
        added = []
        for i in range(1, len(ln_lst)):
            if ln_lst[i] not in added:
                hist_type[ln_lst[i]] = hist_type.get(ln_lst[i], 0) + 1
                added.append(ln_lst[i])

hist_el = dict(sorted(hist_el.items()))
hist_type = dict(sorted(hist_type.items()))

# An "interface" to matplotlib.axes.Axes.hist() method
plt.bar(hist_el.keys(), hist_el.values(), 1.0, color='#0504aa')
plt.xlabel('Number of Elements')
plt.ylabel('Frequency')
plt.title('Frequency of Elements')
plt.text(23, 45, r'$\mu=15, b=3$')
plt.show()

plt.bar(hist_type.keys(), hist_type.values(), 1.0, color='#0504aa')
plt.xlabel('Number of Types')
plt.ylabel('Frequency')
plt.title('Frequency of Types')
plt.text(23, 45, r'$\mu=15, b=3$')
plt.show()