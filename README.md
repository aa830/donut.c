:doughnut:





The famous `donut.c` program, but better!

`Copyright (c) Aarav Darbari <aarvd2030@gmail.com>`



![](https://github.com/aa830/homebrew-donut.c/blob/master/ezgif-5-1fb6afb0e7.gif)






# So whats the difference?

Now, you can choose the colour that you want your donut to be, simply by running an argument with the hexcode of whatever colour you want!

# Installation

## Homebrew

`brew tap aa830/formulae`


`brew install donut`


# Usage
For example, if I wanted to run donut with a standard hex shade of red:

`donut --FF0000`

I can also stack two hexcodes to make the donut transition between the two colours.

Just be sure to add "--" as the argument before your hexcode.

You now also have the `--rainbow` argument! Just run and see what it looks like!

More updates to come!

You can now change the speed that the donut spins at by typing:

`donut --speed <VALUE>`

You can also stack arguments, I can use a `--rainbow --speed 5` argument, which will use the rainbow colour scheme on the donut but also spin it at a speed of 5.

And now! You can change the radius of your donut, using `--radius`. The lower the value the thiccer the donut will get!

**For more details, use `donut --help` or `man donut`**



# Credits

Credit for most of the original script goes to [a1k0n] (https://github.com/a1k0n) and [gcr] (https://github.com/gcr). Without you guys, there would be no donut :sob:
