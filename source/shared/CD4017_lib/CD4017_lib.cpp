class CD4017_LIB {
    unsigned int counter = 0;
    unsigned int steps = 10;
    
    CD4017_LIB (){

    }

    ~CD4017_LIB (){
        
    }

    int reset(){
        return 0;
    }

    int step(){
        return this->counter++;
    }
};
