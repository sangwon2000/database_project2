int move_records(off_t leaf_off);
void move_right(off_t leaf_off, off_t neighbor_off, off_t parent_off, int index, int amount);
void move_left(off_t leaf_off, off_t neighbor_off, off_t parent_off, int index, int amount);

int move_records(off_t leaf_off) {
    
    page *leaf = load_page(leaf_off);
    off_t parent_off = leaf->parent_page_offset;
    page *parent = load_page(parent_off);
    off_t neighbor_off;
    page *neighbor;
    int index = 0, flag = 0;

    if(hp->rpo == leaf_off) return 0;

    if(parent->next_offset == leaf_off) {
        flag = 1;
        neighbor_off = parent->b_f[0].p_offset;
    }
    else if(parent->b_f[0].p_offset == leaf_off) {
        neighbor_off = parent->next_offset;
    }
    else {
        while(parent->b_f[index].p_offset != leaf_off) index ++;
        neighbor_off = parent->b_f[index - 1].p_offset;
    }
    neighbor = load_page(neighbor_off);
    int neighbor_num_of_keys = neighbor->num_of_keys; 

    free(leaf);
    free(parent);
    free(neighbor);

    int amount = (LEAF_MAX - neighbor_num_of_keys) / 2;
    if(amount == 0) return 0;
    
    if(flag == 1) move_right(leaf_off, neighbor_off, parent_off, index, amount);
    else move_left(leaf_off, neighbor_off, parent_off, index, amount);
    return 1;

}

void move_right(off_t leaf_off, off_t neighbor_off, off_t parent_off, int index, int amount) {

    page *leaf = load_page(leaf_off);
    page *neighbor = load_page(neighbor_off);
    page *parent = load_page(parent_off);

    for(int i = neighbor->num_of_keys + amount - 1; i >= amount; i--)
        neighbor->records[i] = neighbor->records[i - amount];

    for(int i = 0; i < amount ; i++) {
        neighbor->records[i] = leaf->records[leaf->num_of_keys - amount + i];
    }

    parent->b_f[index].key = neighbor->records[0].key;
    leaf->num_of_keys -= amount;
    neighbor->num_of_keys += amount;

    pwrite(fd, leaf, sizeof(page), leaf_off);
    pwrite(fd, neighbor, sizeof(page), neighbor_off);
    pwrite(fd, parent, sizeof(page), parent_off);
    
    free(leaf);
    free(neighbor);
    free(parent);
    
    return;

}
void move_left(off_t leaf_off, off_t neighbor_off, off_t parent_off, int index, int amount) {

    page *leaf = load_page(leaf_off);
    page *neighbor = load_page(neighbor_off);
    page *parent = load_page(parent_off);

    for(int i = 0; i < amount ; i++) {
        neighbor->records[neighbor->num_of_keys + i] = leaf->records[i];
    }

    for(int i = 0; i < leaf->num_of_keys - amount; i++)
       leaf->records[i] = leaf->records[amount + i];

    parent->b_f[index].key = leaf->records[0].key;
    leaf->num_of_keys -= amount;
    neighbor->num_of_keys += amount;

    pwrite(fd, leaf, sizeof(page), leaf_off);
    pwrite(fd, neighbor, sizeof(page), neighbor_off);
    pwrite(fd, parent, sizeof(page), parent_off);
    
    free(leaf);
    free(neighbor);
    free(parent);

    return;

}