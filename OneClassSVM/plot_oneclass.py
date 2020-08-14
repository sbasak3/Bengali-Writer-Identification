import numpy as np
import pickle

X =[]
features = []
filename='writer_1.txt'
with open(filename,'r') as fp:
    for line in fp:
        X.append(line.split())
linecount= len(X)
for i in range (0, linecount):
    features.append(X[i][0:])
X_test=np.array(features)

clf = pickle.load(open('One_class_SVM_model.sav', 'rb'))
y_pred_test = clf.predict(X_test)

count=0
for i in range (0,len(y_pred_test)-1):
    if y_pred_test[i] == 1:
        count=count+1;

output_file = open('SVM_Vote_Result.txt','a+')
output_file.write("Writer Name: %s    "%filename)
output_file.write("Vote: %d\n"%count)
output_file.close()