LinkerDB
========

#### This is a Database I created to use for my self

## LinkerDB Commands
 * `newdb` (name) - This will create a new database with the name specified
 * (DBname->link) `put` (key) = (value) - This will put your key with its value into the specified link
 * (DBname->link) `put` (key),(key),(key) = (value),(value),(value) - This will put your key's with their value's into the specified link
 * (DBname->link) `get` (key) - This will get your key from the specified link
 * (DBname->link) `get` (key),(key),(key) - This will get your key's from the specified link
 * (DBname->link) `newlink` (newlink name) - This will create a new link 
 * `print` (DBname->link) - This will print all the data stored in that link, i.e (key) = (value)
 * `desc` (DBname->link) - This will print all the links inside that link

## Example
`newdb Rhythm` - returns OK

`Rhythm newlink user` - returns OK

`Rhythm->user newlink steve` - returns OK

`Ryhthm->user->steve put age = 22` - returns OK

`Ryhthm->user->steve put sex = male` - returns OK

`Rhythm->user newlink john` - returns OK

`Rhythm->user->john put age,sex = 25,male` - returns OK

`desc Rhythm->user` - returns (steve, john)

`print Rhythm->user->steve` - returns (age = 22, sex = male)

`Rhythm->user get age` - returns null

`Rhythm->user->john get age` - returns (age = 25)

`Rhythm->user->john get age,sex` - returns (age = 25, sex = male)
