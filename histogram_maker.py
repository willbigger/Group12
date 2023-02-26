"""
Takes a txt file of advertisers and elements and 
displays two histograms of the frequency from each advertsier
"""



with open("test.txt") as f:
    for line in f.readlines():
        print(line)

