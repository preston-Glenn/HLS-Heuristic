
// Save cost
double cost_function(){
  return AREA * AREA_WEIGHT + LATENCY * ( 1 - AREA_WEIGHT);
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
        if(count > 50){
	        logger.log("UNABLE TO FIND NEW ORGANISM\nEXITING ");
	      exit(1);
        } else {
            return random_organism(count + 1);
        }
    } else{
        return organism;
    }

}


vector<string> crossOver(vector<string> p0, vector<string> p1){
    // Try examples from DSEFrame
    vector<string> child;
    int cutOff = rand() % class_count;
    logger.log("\tCrossover ocurred at: " + int_to_string(cutOff));

    for(int i = 0; i < cutOff; i++){
        child.push_back(p0.at(i));
    }
    for(int i = cutOff; i < class_count; i++){
        child.push_back(p1.at(i));
    }
    return child;
}

vector<string> mutate(vector<string> child,int count = 0){
    vector<string> mutated_child = child;

    for(int i = 0; i < class_count; i++){
        if(rand() % 100 < MUTATION_RATE * 100){
            string str = "attr"+ int_to_string(i+1);
            string prev_attr = mutated_child.at(i);
            string new_attribute = prev_attr;        

            while(prev_attr == new_attribute){
                new_attribute = propertyLists[str][rand() % propertyLists[str].size()];
                cout << "mutation was already present" << endl;
            }
            logger.log("\tMutated @ index: "+int_to_string(i)+" from " + prev_attr + " to " + new_attribute);
	        mutated_child.at(i) = new_attribute;

        } else {
            logger.log("\tDidn't mutate @ index: " + int_to_string(i));
        }
    }
    // // checking if organism has already been made
    // string listsString = listToString(mutated_child);
    // if(attributeMap[listsString] == 1){      // list has already been made
    //     if(count > 20){
    //         logger.log("UNABLE TO FIND NEW ORGANISM\nEXITING ");
    //         exit(1);
    //     } else {
    //         return mutate(child,count+1);
    //     }
    // } else{
    //     return mutated_child;
    // }

    return mutated_child;

}

bool genetic_heuristic(int numberOfRuns){
    Logger score_tracker;
    score_tracker.setFileName("score_tracker.csv");

    int run_count = 0;
    // Generate two random parents that are string vectors of attributes
    vector<string> parent_0 = random_organism();
    vector<string> parent_1 = random_organism();

    int score_parent_0 = 0;
    int score_parent_1 = 0;
    int child_score = 0;

    logger.log("\tGenerated Parent_0:\n\t\t"+listToString(parent_0)+"\n\t\tScore: "+int_to_string(score_parent_0));
    logger.log("\tGenerated Parent_1:\n\t\t"+listToString(parent_1)+"\n\t\tScore: "+int_to_string(score_parent_1));

    // Create new child (vector) through cross over
    vector<string> child = parent_0;

    synthesize(parent_0);
    score_parent_0 = cost_function();

    synthesize(parent_1);
    score_parent_1 = cost_function();

    while(NUMBER_OF_RUNS < numberOfRuns){

        logger.log("\tParent_0:\n\t\t"+listToString(parent_0)+"\n\t\tScore: "+int_to_string(score_parent_0));
        logger.log("\tParent_1:\n\t\t"+listToString(parent_1)+"\n\t\tScore: "+int_to_string(score_parent_1));


        child.clear();
        child = crossOver(parent_0,parent_1);
        child = mutate(child);
        synthesize(child);
        child_score = cost_function();

        logger.log("\tGenerated Child:\n\t\t"+listToString(child)+"\n\t\tScore: "+int_to_string(child_score));

        score_tracker.log(int_to_string(child_score));

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
        run_count++;
    }
    cout << "Finished Meta Heuristic" << endl;
