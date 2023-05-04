Note that you can run into problems if you're using the provided .o files and your types aren't the same size. If you do run into trouble, we have a couple of suggestions:

1) just ignore the provided .o code, or
2) don't use inline functions, start with the driver, and end with the couriers.

You *may* be okay using the .o files if you stick with the PImpl idiom; however, be aware that if your objects are of a size different from those in the .o files, you may get strange behaviour. For the same reason, be aware that I used forward declarations wherever possible to break include cycles. My driver created, in order: printer, bank, parent, WatCard office, Groupoff, nameserver, timer, train stops, trains, conductors, and students. The WatCard office creates the couriers. The random number generator mprng was declared to be extern in the header files. The only includes in the header files were the bare minimum necessary i.e. where a method used/returned a specific class type.
---
Student asked if order of class description was suggestion for development order, and for tips on how to approach writing the classes:

Not really, it's more of a descriptional stream-of-conscience flow.

There are certain pieces that you can code up and test independently, esp. if you do things like have most of your public methods start off empty and your driver yields for a bit before invoking destructors. For example, get the main framework of your tasks structured appropriately. In no particular order, here are some small chunks you can work on, to get the basic structure in place:

    parent deposits money
    train stops register themselves
    train requests lists of stops (but doesn't travel, yet)
    student determines how many trips to make, and whether or not will try to skip paying (but doesn't make trips, yet)
    conductor calls the appropriate train
    timer runs until all students are done
    implement a WatCard and figure out how to work with futures
    implement the WatCard office after you have WatCards working
    create the couriers and have them request work; then once that's working, have them talk to the bank

Once you have the basic framework in place, start adding in the missing functionality.
