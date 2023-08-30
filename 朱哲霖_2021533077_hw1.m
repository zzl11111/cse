K=toeplitz([2 -1 zeros(1,3)]);
e=eig(K)
ans=2*ones(5,1)-2*cos([1:5]*pi/6)'
delta=e-ans