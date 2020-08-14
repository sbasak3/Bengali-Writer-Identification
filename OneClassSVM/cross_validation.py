import numpy as np
from sklearn import svm
from sklearn.model_selection import cross_val_score

X =[]
features = []
filename='training_out464_without_label.txt'
with open(filename,'r') as fp:
    for line in fp:
        X.append(line.split())
linecount= len(X)
for i in range (0, linecount):
    features.append(X[i][0:])
X_data=np.array(features)

target = []
for i in range (0,(len(X_data)/5)):
    target.append(0)
for i in range (0,(len(X_data)/5)):
    target.append(1)
for i in range (0,(len(X_data)/5)):
    target.append(2)
for i in range (0,(len(X_data)/5)):
    target.append(3)
for i in range (0,(len(X_data)-len(target))):
    target.append(4)
X_target=np.array(target)

g=0.001
while g<10:
    clf = svm.OneClassSVM(nu=0.1, kernel="rbf", gamma=g)
    clf.fit(X_data) 
    scores = cross_val_score(clf, X_data, X_target, cv=5, scoring="accuracy")
    print("Gamma Value: %f"%(g))
    print(scores)
    print("Accuracy: %f" % (scores.mean()*100))
    g=g*5