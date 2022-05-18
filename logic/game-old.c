




//int is_changed = 0;



//Node* create_node_struct();
//void add_snake_tail_on_init(Snake* snake,float x, float y);
//void check_out_of_board_end(int x, int y);
//void print_cells_state();
//void add_item(CellState item);
//void remove_from_snake_tail();
//Cell* get_board_cell(int row, int col);
//void move(Cell *next);
//void move_and_add(Cell *next);
//void change_parts();























//Node* node_add_new_head(Node* node, float x, float y){
//    Node* new = create_node_struct();
//    new->next = node;
//    new->p.x = x;
//    new->p.y = y;
//    return new;
//}

//void snake_new_head(Direction direction){
//    //print_cells_state();
//
//    merge_cell_state(x, y, SNAKE_HEAD);
//    int xx = (int)get_snake()->head->p.x;
//    int yy = (int)get_snake()->head->p.y;
//    game->board->cells[xx][yy].state = SNAKE_PART;
//
//    game->board->snake->head = node_add_new_head(get_snake()->head, x, y);
//    char log[100];
//    sprintf(log, "New head cell column : %d, row %d", (int)x, (int)y);
//    logger("INFO", log);
//
//}









//
//void add_snake_tail(float x, float y){
//    add_new_to_tail(get_snake()->head, x,y);
//    get_snake()->size++;
//}



//void change_parts(){
//    Node* node = get_snake()->head->next;
//    ImgType tmp;
//    while (node->next != NULL){
//        tmp = node->next->type;
//        node->next->type = node->type;
//    }
//}
//


