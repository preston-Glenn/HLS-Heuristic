
// Save cost
double cost_function(int area, int latency, double weight_of_area){
    return area * weight_of_area + latency * (1 - weight_of_area);
}


vector<string> random_organism(int count = 0){
    vector<string>  organism;
    

    for(int j = 1; j <= class_count; j++){
        string str = "attr"+ int_to_string(j);
        string new_attribute = propertyLists[str][rand() % propertyLists[str].size()];
        organism.push_back(new_attribute);
    }

    // checking if organism has already been made
    string listsString = listToString( organism);
    if(attributeMap[listsString] == 1){      // list has already been made
        if(count > 20){
	  logger.log("UNABLE TO FIND NEW ORGANISM\nEXITING ");
	      exit(1);
        } else {
            return random_organism(count++);
        }
    } else{
        return organism;
    }

}


vector<string> crossOver(vector<string> p1, vector<string> p2){
    // Try examples from DSEFrame
    vector<string> child;
    int cutOff = rand() % class_count;

    for(i = 0; i < cutOff; i++){
        child.push_back(p0.at(i));
    }
    for(i = cutOff; i < class_count; i++){
        child.push_back(p1.at(i));
    }
    return child;
}

vector<string> mutate(vector<string> child){

    for(int i = 0; i < class_count; i++){
        if(rand() % 100 < MUTATION_RATE * 100){
            
	  string str = "attr"+ int_to_string(i+1);
	  string prev_attr = child.at(i);
	  string new_attribute = prev_attr;        

	  while(prev_attr == new_attribute){
	    new_attribute = propertyLists[str][rand() % propertyLists[str].size()];
            cout << "mutation was already present" << endl;
	  }

	  child.at(i) = new_attribute;

        }
    }
    return child;

}

bool genetic_heuristic(int numberOfRuns){
    int run_count = 0;
    // Generate two random parents that are string vectors of attributes
    vector<string> parent_0 = random_organism();
    vector<string> parent_1 = random_organism();

    int score_parent_0 = 0;
    int score_parent_1 = 0;
    int child_score = 0;

    logger.log("Generated Parent_0:\n\t"+listToString(parent_0)+"\n\tScore: "+int_to_string(score_parent_0));
    logger.log("Generated Parent_1:\n\t"+listToString(parent_1)+"\n\tScore: "+int_to_string(score_parent_1));

        // Create new child (vector) through cross over
    vector<string> child = parent_0;
    score_parent_0 = synthesize(parent_0);
    score_parent_1 = synthesize(parent_1);

    while(count < numberOfRuns){

      child.clear();
        child = crossOver(parent_0,parent_1);
        child = mutate(child);

        child_score = synthesize(child);

        if(child_score < score_parent_0 && child_score < score_parent_1 ){
            if(score_parent_0 > score_parent_1){
                parent_0 = child;
                score_parent_0 = child_score;
            } else {
                parent_1 = child;
                score_parent_1 = child_score;
            }
        } else if(child_score < score_parent_0 && child_score > score_parent_1 ){
            parent_0 = child;
            score_parent_0 = child_score;
        } else if(child_score > score_parent_0 && child_score < score_parent_1 ){
            parent_1 = child;
            score_parent_1 = child_score;
        }
        count++;
    }

        // Iterate through child vector and mutate ~10%

        // calculate child cost

        // replace parent with biggest cost with child



}
