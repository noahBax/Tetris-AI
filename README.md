# Tetris-AI
An implementation for training my Artificial Intelligence Tetris bot

## Contents
This repository has a collection of C++ files that are designed to train the weights for a Tetris bot that I created that runs in the browser.
It also contains a simple web page that can demonstrate the results of training.

Check inside each folder for a more comprehensive overview of each component.

[Check out a live demo of the product](https://noahbaxley.com/Tetris/)

## Backstory for this project
Here's some context for why I built these things in the first place. This is a bit of a ramble.
In October of 2023, I decided I wanted to build a game of Tetris.
So, I pulled an all nighter and finished the game in about 3 hours.
Why was it an all nighter? Because the rest of the night was spent styling the game and going back and forth with my roommate, who was an
avid Tetris player in his teens, to try to get the look and feel of the game right. In the end, I reached a point where I was
satisfied with the end product.

It was only after programming Tetris that I realized I was bad at it. I couldn't compare to my dad, my roommate, or even my mother (who
is notoriously bad at video games I'll have you know).
So, I decided I should create an AI to do it. To allow me to compete with my self-proclaimed rivals.

## How this was accomplished
Despite my AI degree, I am relatively new to this specific kind of thing. I have taken AI classes in college but none that would apply to
this situation. So, I created a list of requirements for myself.
1) I need to find a way to judge how good a given move in Tetris is.
2) I need to create an AI algorithm capable of using this judgement method.
3) I need to find a way to train my AI so it can _learn_ how to make better decisions as time passes.

As always, the first thing to do in this scenario is research. I searched around and found a few papers. [This paper](https://kth.diva-portal.org/smash/get/diva2:815662/FULLTEXT01.pdf)
was very insightful and I found [this guy's report](https://codemyroad.wordpress.com/2013/04/14/tetris-ai-the-near-perfect-player/#:~:text=The%20score%20for%20each%20move,to%20either%20minimize%20or%20maximize.)
to be very helpful as well.

To summarize the content of both, the AI makes intelligent decisions by computing all possible scenarios that can be made by dropping
a piece in every position/orientation from the top of the board. It then assigns a bad-ness rating to the resulting board based on certain
metrics that you define. The higher a bad-ness rating is, the worse the scenario is viewed as, so the scenario with the lowest rating
is chosen and the piece is dropped from that position. This doesn't allow fancy moves like T-spins of course, but the finished product
does really well despite lacking those things.

An example metric that you would use to judge how bad a scenario is by looking at the number of holes you create by dropping a piece
in a location. For example:

<img src="https://noahbaxley.com/gitImages/tetrisBad.png" style="width: 200px;" alt="Example of a bad move in Tetris. Notice how there are holes created by the piece dropped here"></img>

Dropping the purple piece here creates holes underneath it that cannot be filled. Objectively speaking, it is one of the worst moves you could make
in this scenario.

For my AI, I chose to use over 20 metrics. Some of them are more important than others, but all in all they give my AI a pretty good
understanding of how the Tetris board looks in each scenario. Each metric is multiplied and/or exponentiated by a "weight" which essentially decides how
important that metric is when it comes to scoring the board. Multiply each weight by its associated metric and add them all together and that's
how you get the rating for a board. These weights are what makes one AI different from another AI in my project and
they are what get trained in the training process.

As for training, I chose to use a genetic algorithm. I did some looking around on the internet and found that the general structure for
a genetic algorithm is:
1) Set up a population that will be tested in each generation of training.
2) Test each member of the population to assign it a fitness value. The fitness value is a measure of how well a given AI configuration performed.
3) Sort through the results of the generation and pull out the AI's that did the best. These will be the parents of the next generation.
4) Create the next population by combining the genetic code (configurations in my case) of two parents.
5) Apply some number of mutations to children in the new population.
6) Go back to step 2 and repeat this for the number of generations you wanted.
7) At the very end, pull out the best performing member of your population and that will be your result.

Keep in mind, this is a general overview and steps can be performed in different orders depending on where you look.

Now that a plan has been established, coding can begin.

## Coding the training program
I am most familiar with JavaScript, so I decided (at first) that I would write the training algorithm in JS and do training in NodeJS.
NodeJS is just a way to run JS outside of the browser, so it offers both ease of use _and_ a speed increase over regular JS.
The fact that I originally wrote Tetris in JavaScript to begin with meant I could take a large amount of code from that and put it
directly into my training script which made the process go a lot faster.

Coding up the training algorithm means implementing the genetic algorithm detailed above. The testing phase of the algorithm is done
by taking the population and simulating a game played by each of them and looking at the results. For my case, I tested each
population member by simulating 3 games and taking the average fitness value. I also limited each game to just using 600 pieces so
that training didn't go on indefinitely.

The first measure I chose to use as fitness was the number of lines cleared. So, the best AI would be one that maximizes lines cleared.
You can check out how that turned out [here](https://noahbaxley.com/Tetris/naiveTraining/index.html) if you want. Notice how, yes, it does clear
a lot of lines and it can go on for a LONG time, but it doesn't really have any interest in clearing multiple lines at a time. So, if I
want more lines cleared, a better value to use as fitness might be score. Or the ratio of score / lines. Or score x lines. These are the
fascinating questions you get to think about and test when you do things like this.

Both because I wanted the training to be faster and I wanted to challenge myself, I chose to write another version of the trainer script
in C++. I hadn't actually programmed in C++ before, so this was a unique experience to say the least. After finishing the C++ version,
the training was now going _significantly_ faster, and with much less strain on my PC too. I am very satisfied with how the script turned
out here.

Over the coming months I have added many more metrics to those I had initially and adjusted or improved the originals.
The most current iteration of the Tetris AI is actually 3 individual AIs that work in tandem with each other to create the best scenario possible.

## The result
I am satisfied with how the training algorithms I wrote turned out. They work well and I learned a lot. I wouldn't say I've mastered
genetic algorithms, and I definitely cannot call myself a master at C++ because of this, but I understand both a lot better now.
Above all, I like how cool the AI looks playing by itself on the website application.

I think there are other metrics I might add in the future to help improve the performance or I may add optimizations that would allow
the AI to train faster. Right now, though, I like it!

I highly encourage you to check out the demo! I worked hard on it and am very proud of how far I've come since the beginning.
