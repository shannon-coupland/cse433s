# cse433s

    How to run attack:
First, you need to update the IP adress to reflect your own in
threshold.c and hack.c by changing the "ip" global variable. Then you can run hack_script and perform the attack. If you wish, you may add your own username/password pairs to the CREDS global variable in server.c. You may also train the threshold on different data by updating the USERNAME and PASSWORD global variables in threshold.c -- however, to get a well-trained threshold, you need to provide a long password (say, at least 20 characters).

To save time, we have lowered the number of iterations in threshold.c and hack.c, but this comes at the price of some fluke runs. If the hack doesn't work the first time, try a few more times -- it should work if given enough runs. If you have more time to spare, you may increase the NUM_ITERS global variable in threshold.c and hack.c to increase the chances of a successful attack.
