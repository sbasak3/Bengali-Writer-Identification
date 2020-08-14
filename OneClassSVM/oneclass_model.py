import numpy as np
from sklearn import svm
import pickle

X =[]
features = []
filename='training_out464_without_label.txt'
with open(filename,'r') as fp:
    for line in fp:
        X.append(line.split())
linecount= len(X)
for i in range (0, linecount):
    features.append(X[i][0:])
X_train=np.array(features)

model = svm.OneClassSVM(nu=0.1, kernel="rbf", gamma=3.125)
model.fit(X_train)
modelfile = 'One_class_SVM_model.sav'
pickle.dump(model,open(modelfile,'wb'))