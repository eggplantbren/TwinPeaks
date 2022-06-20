import apsw
import matplotlib.pyplot as plt
import numpy as np

conn = apsw.Connection("output/twinpeaks.db", flags=apsw.SQLITE_OPEN_READONLY)
db = conn.cursor()

xs, ys = [], []
for row in db.execute("SELECT * FROM scalars;"):
    if row[1] == 0:
        xs.append(row[2])
    else:
        ys.append(row[2])

plt.plot(xs, ys, ".")
plt.show()
