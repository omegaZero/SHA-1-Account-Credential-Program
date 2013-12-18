# SHA-1-Authentication

This is just a little project to play around with usernames and password hashes.

~~Right now it only has the ability to recognize new users and display the hash of their desired password. Complete functionality should include being able to authenticate users on file and to add new users as well. It's currently a little messy, but that'll get worked out when I have more time.~~


## New Stuff

I'm currently completely redoing this project. I haven't been able to look at it for months and I'm not satisfied with how I tried to implement it. 

This branch will house all of the new changes, until I feel ready to merge it back with master. Changes will be coming very soon.

## Build/Usage

### Build
```shell
   $ make
```

### Clean
```shell
   $ make clean
```

### Usage
```shell
    $ ./login
```
You're gonna need that './' or else you'll be running your /bin/login
(I know, it's a terrible name for an executable, I'll change it when I think of something better)

## Current goals
1. ~~Define how to read from binary file (User creation and reading elements from file)~~
1. ~~Make function to implement said definitions~~
1. ~~Define how to then write to the file to save state~~
1. ~~Write functions to implement said definitions~~
1. ~~Define how to read passwords, hash and store them~~
1. ~~Implement said functionality, include it into the user account creation flow~~
