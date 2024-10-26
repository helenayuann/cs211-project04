## test04.py ##
#
# int, real, string concat
#
x = 1
y = 10.5
z = "shorter"
x = 2

print(x)
print(y)
print(z)

b = x
c = y
d = z
print(b)
print(c)
print(d)

a = z + " string"
print(a)

b = x + 3.675
c = y + 10
d = x + 1
some_var = "cs"

print(b)
print(c)
print(d)

e = z + "+a very long string of word that could be many many words --- did you dynamically allocate?"
print(e)

f = some_var + " 211"
print(f)

x = 1
y = 10.5
z = "shorter"

a = 10
b = 3.675
c = "cs "
d = "a very long string of word that could be many many words --- did you dynamically allocate? "

e = "211"
print("")

var1 = x + a
var2 = b + y
var3 = c + e
var4 = d + z
var5 = b + x
var6 = a + y

x = x + x
b = b + b
e = e + e

print(var1)
print(var2)
print(var3)
print(var4)
print(var5)
print(var6)
print(x)
print(b)
print(e)

s1 = "apple"
s2 = "APPLE"
s3 = "banana"
s4 = "pear"
s5 = "banana"

b1_1 = s1 == s2
b1_2 = s1 == "APPLE"
b1_3 = s1 == "apple"
b1_4 = s1 == "applesauce"
b1_5 = "APPLE" == s2
b1_6 = "APPLE" == s1
b1_7 = s3 == s5
b1_8 = s3 == s4
b1_9 = s3 == s3

b2_1 = s1 != s2
b2_2 = s1 != "APPLE"
b2_3 = s1 != "apple"
b2_4 = s1 != "applesauce"
b2_5 = "APPLE" != s2
b2_6 = "APPLE" != s1
b2_7 = s3 != s5
b2_8 = s3 != s4
b2_9 = s3 != s3

b3_1 = s1 < s2
b3_2 = s1 < "APPLE"
b3_3 = s1 < "apple"
b3_4 = s1 < "applesauce"
b3_5 = "APPLE" < s2
b3_6 = "APPLE" < s1
b3_7 = s3 < s5
b3_8 = s3 < s4
b3_9 = s3 < s3

b4_1 = s1 > s2
b4_2 = s1 > "APPLE"
b4_3 = s1 > "apple"
b4_4 = s1 > "applesauce"
b4_5 = "APPLE" > s2
b4_6 = "APPLE" > s1
b4_7 = s3 > s5
b4_8 = s3 > s4
b4_9 = s3 > s3

b5_1 = s1 <= s2
b5_2 = s1 <= "APPLE"
b5_3 = s1 <= "apple"
b5_4 = s1 <= "applesauce"
b5_5 = "APPLE" <= s2
b5_6 = "APPLE" <= s1
b5_7 = s3 <= s5
b5_8 = s3 <= s4
b5_9 = s3 <= s3

b6_1 = s1 >= s2
b6_2 = s1 >= "APPLE"
b6_3 = s1 >= "apple"
b6_4 = s1 >= "applesauce"
b6_5 = "APPLE" >= s2
b6_6 = "APPLE" >= s1
b6_7 = s3 >= s5
b6_8 = s3 >= s4
b6_9 = s3 >= s3

