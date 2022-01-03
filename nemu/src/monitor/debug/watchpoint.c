#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
    wp_pool[i].val_pre = 0;
    wp_pool[i].valid = false;
    strcpy(wp_pool[i].expr,"");
  }
  wp_pool[NR_WP - 1].next = NULL;
  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
WP* new_wp(){
  WP* tmp_head = NULL;
  if(free_){
    if(head==NULL){
      head = free_;
      free_ = free_->next;
      head->next = NULL;
      return head;
    }
    else{
      tmp_head = head;
      while(tmp_head->next){
        tmp_head = tmp_head->next;
      }
      tmp_head->next = free_;
      free_ = free_->next;
      tmp_head->next->next = NULL;
      return tmp_head->next;
    }
  }
  else
    return NULL;
}

int free_wp(int id){
  WP* tmp = head;
  WP* free_tmp = free_;
  WP* wp;
  if(head){
    if(tmp->NO == id){
      while(free_tmp->next){
        free_tmp = free_tmp->next;
      }
      head->valid = false;
      head->val_pre = 0;
      strcpy(head->expr,"");
      head = NULL;
      return 1;
    }

    while(tmp->next){
      
      if(tmp->next->NO == id){
        wp = tmp->next;
        tmp->next = wp->next;

        if(free_){
          while(free_tmp->next){
            free_tmp = free_tmp->next;
          }
          free_tmp->next = wp;
        }
        else{
          free_ = wp;
        }
        strcpy(wp->expr,"");
        wp->valid = false;
        wp->val_pre = 0;
        wp->next = NULL;
        return 1;
      }

      tmp = tmp->next;
    }
  }
  else
    return 0;
  return -1;
}

WP* gethead(){
  return head;
}