# Resource Unloader

## How it works

Using the WinAPI and having a resource (any file/binary) loaded into the resource section of an executable, the executable can extract these embedded files from itself.

## How to setup

In order for the resource to be unloaded, the _resource.h_ file must contain:

```cpp
#define TestResource                       101
```
being 101 the ID for the resource, and the _(projectName).rc_ file must contain:

```rc
TestResource               TEXT                    "test.txt"
```