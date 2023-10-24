#include <cairo.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

#include "calculate.h"

typedef struct {
  GtkWidget *area;
  GtkWidget *entry;
  GtkWidget *button_zoom_in;
  GtkWidget *button_zoom_out;
  const gchar *entry_text;
  GtkWidget *x_field;
  GtkWidget *entry_main;
  GtkWidget *window_g;
} Grath;

typedef struct {
  int flag;
  double payment_month[201];
  GtkTextBuffer *text_field;
  GtkWidget *overpayment_field;
  GtkWidget *total_payment_field;
  GtkWidget *button_endd;
  GtkWidget *entry_summ;
  GtkWidget *entry_periodd;
  GtkWidget *entry_bett;
} Credit;

typedef struct {
  int flag;
  GtkWidget *accrued_interest;
  GtkWidget *tax_amount;
  GtkWidget *amount_on_deposit;
  GtkWidget *deposit_amount;
  GtkWidget *month_term;
  GtkWidget *interest_rate;
  GtkWidget *tax_rate;
  GtkWidget *repayment_periodicity;
  GtkWidget *month_in;
  GtkWidget *sum_in;
  GtkTreeView *treeview;
  GtkTreeView *treeview_1;
  GtkListStore *listbox_input;
  GtkListStore *listbox_output;
  GtkWidget *month_out;
  GtkWidget *sum_out;
  GtkWidget *closed_window_in;
  GtkWidget *closed_window_out;
} Deposit;

static void button_clicked(GtkWidget *widget, gpointer data) {
  Grath *graph = (Grath *)data;
  const gchar *text = gtk_button_get_label(GTK_BUTTON(widget));
  if (g_strcmp0(text, "=") == 0) {
    GtkEntry *entry = GTK_ENTRY(graph->entry_main);

    const gchar *entry_text = gtk_entry_get_text(entry);
    double answer;
    char expression[strlen(entry_text) + 1];
    double x_value =
        (double)gtk_spin_button_get_value(GTK_SPIN_BUTTON(graph->x_field));
    strcpy(expression, entry_text);
    int rc = solve_expression(expression, x_value, &answer);
    gchar result[100];
    switch (rc) {
      case 0: {
        g_strlcpy(result, g_strdup_printf("%.6lf", answer), sizeof(result));
        break;
      }
      case 10: {
        g_strlcpy(
            result,
            "Степень нуля не определена для отрицательного показателя степени!",
            sizeof(result));
        break;
      }
      case 11: {
        g_strlcpy(result, "Деление на ноль не определено!", sizeof(result));
        break;
      }
      case 12: {
        g_strlcpy(result, "Извлекать корень из отрицательного числа запрещено!",
                  sizeof(result));
        break;
      }
      case 13: {
        g_strlcpy(result, "На данный момент касательная не определена!",
                  sizeof(result));
        break;
      }
      case 14: {
        g_strlcpy(result, "Логарифм не определен для неположительных чисел!",
                  sizeof(result));
        break;
      }
      case 15: {
        g_strlcpy(
            result,
            "Обратный косинус не определен для значений за пределами [-1, 1]!",
            sizeof(result));
        break;
      }
      case 16: {
        g_strlcpy(
            result,
            "Обратный синус не определен для значений за пределами [-1, 1]!",
            sizeof(result));
        break;
      }
      default: {
        g_strlcpy(result, "Неправильный ввод или другая ошибка!",
                  sizeof(result));
        break;
      }
    }
    gtk_entry_set_text(entry, result);

  } else if (g_strcmp0(text, "AC") == 0) {
    gtk_entry_set_text(GTK_ENTRY(graph->entry_main), "");
  } else if (g_strcmp0(text, "⌫") == 0) {
    const gchar *trig[] = {"asin", "acos", "atan", "sqrt", "sin",
                           "cos",  "tan",  "log",  "ln"};
    GtkEntry *entry = GTK_ENTRY(graph->entry_main);
    if (gtk_entry_get_text_length(entry) > 0) {
      const gchar *text_1 = gtk_entry_get_text(entry);
      size_t text_1_len = strlen(text_1);
      if (text_1_len > 1) {
        for (int i = 0; i < 9; i++) {
          const char *found =
              strstr(text_1 + (text_1_len - strlen(trig[i])), trig[i]);
          if (found != NULL &&
              found == text_1 + (text_1_len - strlen(trig[i]))) {
            if (strlen(trig[i]) == 4) {
              gchar *new_text = g_strndup(text_1, strlen(text_1) - 3);
              gtk_entry_set_text(entry, new_text);
              g_free(new_text);
            } else if (strlen(trig[i]) == 3) {
              gchar *new_text = g_strndup(text_1, strlen(text_1) - 2);
              gtk_entry_set_text(entry, new_text);
              g_free(new_text);
            } else {
              gchar *new_text = g_strndup(text_1, strlen(text_1) - 1);
              gtk_entry_set_text(entry, new_text);
              g_free(new_text);
            }
            break;
          }
        }
      }
      gchar *new_text = g_strndup(text_1, strlen(text_1) - 1);
      gtk_entry_set_text(entry, new_text);
      g_free(new_text);
    }
  } else {
    GtkEntry *entry = GTK_ENTRY(graph->entry_main);
    const gchar *current_text = gtk_entry_get_text(entry);
    const char *found = strstr(current_text + (strlen(current_text) - 1), "!");
    if (found != NULL && found == current_text + (strlen(current_text) - 1)) {
      gtk_entry_set_text(GTK_ENTRY(graph->entry_main), "");
    }
    gchar *new_text = g_strconcat(current_text, text, NULL);
    gtk_entry_set_text(entry, new_text);
    g_free(new_text);
  }
}

void draw_axes_and_grid(cairo_t *cr, double width, double height, double min_x,
                        double max_x, double min_y, double max_y, int count) {
  cairo_set_source_rgb(cr, 0.2, 0.2, 0.2);
  cairo_set_line_width(cr, 1);

  if (max_y - min_y < height && max_x - min_x < width) {
    for (double y = min_y; y <= max_y; y += (max_y - min_y) / count) {
      double y_normalized = height - (y - min_y) / (max_y - min_y) * height;
      cairo_move_to(cr, 0, y_normalized);
      cairo_line_to(cr, width, y_normalized);
      cairo_stroke(cr);
    }

    for (double x = min_x; x <= max_x; x += (max_x - min_x) / count) {
      double x_normalized = (x - min_x) / (max_x - min_x) * width;
      cairo_move_to(cr, x_normalized, 0);
      cairo_line_to(cr, x_normalized, height);
      cairo_stroke(cr);
    }
  }

  cairo_set_source_rgb(cr, 0.5, 0.5, 0.5);
  cairo_set_line_width(cr, 2);

  double x_axis = -min_y / (max_y - min_y) * height;
  cairo_move_to(cr, 0, height - x_axis);
  cairo_line_to(cr, width, height - x_axis);

  double y_axis = (0 - min_x) / (max_x - min_x) * width;
  cairo_move_to(cr, y_axis, 0);
  cairo_line_to(cr, y_axis, height);

  cairo_stroke(cr);
}

gboolean draw_callback(GtkWidget *widget, cairo_t *cr, gpointer data) {
  Grath *graph = (Grath *)data;

  const gchar *entry_text = graph->entry_text;
  int width = 600;
  (void)widget;
  char expression[strlen(entry_text) + 1];
  strcpy(expression, entry_text);

  double coordinates[1000][2];
  double l =
      (double)gtk_spin_button_get_value(GTK_SPIN_BUTTON(graph->button_zoom_in));
  double r = (double)gtk_spin_button_get_value(
      GTK_SPIN_BUTTON(graph->button_zoom_out));
  double step = (r - l) / width;
  int rc = get_graph(expression, l, r, step, coordinates);
  gchar result[100];
  switch (rc) {
    case 0: {
      g_strlcpy(result, "Graph", sizeof(result));

      int width = 600, height = 600;

      cairo_set_source_rgb(cr, 0, 0, 0);
      cairo_paint(cr);

      double min_x = coordinates[0][0];
      double max_x = coordinates[width - 1][0];
      double min_y = min_x;
      double max_y = max_x;

      cairo_set_source_rgb(cr, 0, 0, 0);
      cairo_paint(cr);

      draw_axes_and_grid(cr, width, height, min_x, max_x, min_y, max_y,
                         (int)(r - l));

      for (int i = 0; i < width; i++) {
        coordinates[i][0] =
            (coordinates[i][0] - min_x) / (max_x - min_x) * width;
        coordinates[i][1] =
            height - (coordinates[i][1] - min_y) / (max_y - min_y) * height;
      }

      cairo_set_source_rgb(cr, 1, 1, 1);
      cairo_set_line_width(cr, 2);
      cairo_stroke(cr);
      cairo_move_to(cr, coordinates[0][0], coordinates[0][1]);
      for (int i = 1; i < width; i++) {
        if (isnan(coordinates[i][1]) || coordinates[i][1] >= height ||
            coordinates[i][1] <= 0) {
          cairo_stroke(cr);
        } else {
          cairo_line_to(cr, coordinates[i][0], coordinates[i][1]);
        }
      }

      cairo_stroke(cr);
      break;
    }
    case 40: {
      g_strlcpy(result, "Правый край должен превышать левый край",
                sizeof(result));
      break;
    }
    case 10: {
      g_strlcpy(
          result,
          "Степень нуля не определена для отрицательного показателя степени",
          sizeof(result));
      break;
    }
    case 11: {
      g_strlcpy(result, "Деление на ноль не определено", sizeof(result));
      break;
    }
    case 12: {
      g_strlcpy(result,
                "Извлечение корня из отрицательного числа не допускается",
                sizeof(result));
      break;
    }
    case 13: {
      g_strlcpy(result, "На данный момент касательная не определена",
                sizeof(result));
      break;
    }
    case 14: {
      g_strlcpy(result, "Логарифм не определен для неположительных чисел",
                sizeof(result));
      break;
    }
    case 15: {
      g_strlcpy(
          result,
          "Обратный косинус не определен для значений за пределами [-1, 1]",
          sizeof(result));
      break;
    }
    case 16: {
      g_strlcpy(result,
                "Обратный синус не определен для значений за пределами [-1, 1]",
                sizeof(result));
      break;
    }
    default: {
      g_strlcpy(result, "Неправильный ввод или другая ошибка", sizeof(result));
      break;
    }
  }
  gtk_entry_set_text(GTK_ENTRY(graph->entry), result);

  return FALSE;
}

void button_draw_graph(GtkWidget *button, Grath *grath) {
  (void)button;
  gtk_widget_queue_draw(grath->area);
}

void list_store(GtkWidget *widget, gpointer data) {
  double overpayment;
  double total_payment;
  (void)widget;
  Credit *credit = (Credit *)data;
  gtk_text_buffer_set_text(credit->text_field, "", -1);
  double sum =
      (double)gtk_spin_button_get_value(GTK_SPIN_BUTTON(credit->entry_summ));
  int period =
      (int)gtk_spin_button_get_value(GTK_SPIN_BUTTON(credit->entry_periodd));
  double bet =
      (double)gtk_spin_button_get_value(GTK_SPIN_BUTTON(credit->entry_bett));
  int rc = calc_credit(sum, period, bet, credit->flag, credit->payment_month,
                       &overpayment, &total_payment);
  if (!rc) {
    for (int i = 0; i < period; ++i) {
      char elem[100];
      sprintf(elem, "%d:  %f\n", i + 1, credit->payment_month[i]);

      GtkTextIter iter;
      gtk_text_buffer_get_end_iter(credit->text_field, &iter);
      gtk_text_buffer_insert(credit->text_field, &iter, elem, -1);
    }
    char overpayment_str[100];
    char total_payment_str[100];
    sprintf(overpayment_str, "Переплата: %.2f", overpayment);
    sprintf(total_payment_str, "Весь платеж: %.2f", total_payment);
    gtk_label_set_text(GTK_LABEL(credit->overpayment_field), overpayment_str);
    gtk_label_set_text(GTK_LABEL(credit->total_payment_field),
                       total_payment_str);
  }
}

void radio_button_toggled(GtkToggleButton *button, gpointer data) {
  Credit *credit = (Credit *)data;
  if (gtk_toggle_button_get_active(button)) {
    const gchar *label = gtk_button_get_label(GTK_BUTTON(button));
    size_t text_1_len = strlen(label);
    const char *found =
        strstr(label + (text_1_len - strlen("Дифференцированные")),
               "Дифференцированные");
    if (found != NULL &&
        found == label + (text_1_len - strlen("Дифференцированные"))) {
      credit->flag = 1;

    } else {
      credit->flag = 0;
    }
  }
}

static void button_credits(GtkWidget *widget) {
  Credit *credit = g_malloc(sizeof(Credit));
  credit->flag = 0;
  (void)widget;
  GtkWidget *window_credits = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window_credits), "Кредит");
  gtk_window_set_default_size(GTK_WINDOW(window_credits), 500, 400);
  gtk_container_set_border_width(GTK_CONTAINER(window_credits), 10);
  gtk_window_set_resizable(GTK_WINDOW(window_credits), FALSE);

  GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_container_add(GTK_CONTAINER(window_credits), vbox);

  GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);

  GtkWidget *grid_1 = gtk_grid_new();
  gtk_grid_set_row_spacing(GTK_GRID(grid_1), 5);
  gtk_grid_set_column_spacing(GTK_GRID(grid_1), 5);
  gtk_box_pack_start(GTK_BOX(hbox), grid_1, TRUE, TRUE, 0);

  GtkWidget *label = gtk_label_new("Сумма");
  gtk_grid_attach(GTK_GRID(grid_1), label, 0, 0, 1, 1);
  GtkWidget *deposit_amount = gtk_spin_button_new_with_range(0, 10000000, 1);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(deposit_amount), 10000);
  gtk_grid_attach(GTK_GRID(grid_1), deposit_amount, 1, 0, 1, 1);
  credit->entry_summ = deposit_amount;

  GtkWidget *label_1 = gtk_label_new("Период в мес");
  gtk_grid_attach(GTK_GRID(grid_1), label_1, 0, 1, 1, 1);
  GtkWidget *entry_period = gtk_spin_button_new_with_range(0, 1000, 1);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(entry_period), 12);
  gtk_grid_attach(GTK_GRID(grid_1), entry_period, 1, 1, 1, 1);

  credit->entry_periodd = entry_period;

  GtkWidget *label_2 = gtk_label_new("Ставка");
  gtk_grid_attach(GTK_GRID(grid_1), label_2, 0, 2, 1, 1);
  GtkWidget *entry_bet = gtk_spin_button_new_with_range(0, 1000, 1);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(entry_bet), 5);
  gtk_grid_attach(GTK_GRID(grid_1), entry_bet, 1, 2, 1, 1);
  credit->entry_bett = entry_bet;

  GtkWidget *label_3 = gtk_label_new("Итог");
  gtk_grid_attach(GTK_GRID(grid_1), label_3, 3, 0, 1, 1);
  GtkWidget *textView = gtk_text_view_new();
  GtkTextBuffer *text_field = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));
  gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textView), GTK_WRAP_WORD);
  gtk_text_view_set_editable(GTK_TEXT_VIEW(textView), FALSE);

  GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER(scroll), textView);
  gtk_widget_set_size_request(scroll, 200, 200);
  gtk_box_pack_start(GTK_BOX(hbox), scroll, TRUE, TRUE, 0);
  credit->text_field = text_field;

  GtkWidget *button1 = gtk_radio_button_new_with_label(NULL, "Аннуитетные");
  GtkWidget *button2 = gtk_radio_button_new_with_label_from_widget(
      GTK_RADIO_BUTTON(button1), "Дифференцированные");

  g_signal_connect(G_OBJECT(button1), "toggled",
                   G_CALLBACK(radio_button_toggled), credit);
  g_signal_connect(G_OBJECT(button2), "toggled",
                   G_CALLBACK(radio_button_toggled), credit);

  gtk_grid_attach(GTK_GRID(grid_1), button1, 1, 3, 1, 1);
  gtk_grid_attach(GTK_GRID(grid_1), button2, 1, 4, 1, 1);

  credit->overpayment_field = gtk_label_new(NULL);
  gtk_grid_attach(GTK_GRID(grid_1), credit->overpayment_field, 0, 5, 1, 1);

  credit->total_payment_field = gtk_label_new(NULL);
  gtk_grid_attach(GTK_GRID(grid_1), credit->total_payment_field, 0, 6, 1, 1);

  GtkWidget *button_end = gtk_button_new_with_label("Расчитать");
  gtk_box_pack_start(GTK_BOX(vbox), button_end, FALSE, FALSE, 0);
  credit->button_endd = button_end;
  g_signal_connect(G_OBJECT(credit->button_endd), "clicked",
                   G_CALLBACK(list_store), credit);
  gtk_widget_show_all(window_credits);
}

void on_checkbox_toggled(GtkWidget *widget, gpointer data) {
  Deposit *deposit = (Deposit *)data;
  gboolean checked = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget));
  if (checked) {
    deposit->flag = 1;
  } else {
    deposit->flag = 0;
  }
}

void add_data_in(GtkWidget *widget, gpointer data) {
  Deposit *deposit = (Deposit *)data;
  (void)widget;
  gtk_window_close(GTK_WINDOW(deposit->closed_window_in));
  const gchar *month = gtk_entry_get_text(GTK_ENTRY(deposit->month_in));
  const gchar *sum = gtk_entry_get_text(GTK_ENTRY(deposit->sum_in));
  int month_int = atoi(month);
  double sum_double = atof(sum);
  if (month_int != 0 && sum_double != 0) {
    GtkTreeIter iter;
    gtk_list_store_append(GTK_LIST_STORE(deposit->listbox_input), &iter);
    gtk_list_store_set(GTK_LIST_STORE(deposit->listbox_input), &iter, 0,
                       month_int, 1, sum_double, -1);
  }
}

void add_data_out(GtkWidget *widget, gpointer data) {
  Deposit *deposit = (Deposit *)data;
  (void)widget;
  gtk_window_close(GTK_WINDOW(deposit->closed_window_out));
  const gchar *month = gtk_entry_get_text(GTK_ENTRY(deposit->month_out));
  const gchar *sum = gtk_entry_get_text(GTK_ENTRY(deposit->sum_out));
  int month_int = atoi(month);
  double sum_double = atof(sum);
  if (month_int != 0 && sum_double != 0) {
    GtkTreeIter iter;
    gtk_list_store_append(GTK_LIST_STORE(deposit->listbox_output), &iter);
    gtk_list_store_set(GTK_LIST_STORE(deposit->listbox_output), &iter, 0,
                       month_int, 1, sum_double, -1);
  }
}

void remove_data_in(GtkWidget *widget, gpointer data) {
  Deposit *deposit = (Deposit *)data;
  (void)widget;
  GtkTreeIter iter;
  GtkTreeIter last_iter;
  gint count = gtk_tree_model_iter_n_children(
      GTK_TREE_MODEL(deposit->listbox_input), NULL);
  if (count > 0) {
    gboolean result = gtk_tree_model_get_iter_first(
        GTK_TREE_MODEL(deposit->listbox_input), &iter);
    if (count > 1) {
      if (result) {
        while (gtk_tree_model_iter_next(GTK_TREE_MODEL(deposit->listbox_input),
                                        &iter)) {
          last_iter = iter;
        }
        gtk_list_store_remove(deposit->listbox_input, &last_iter);
      }
    } else {
      gtk_list_store_remove(deposit->listbox_input, &iter);
    }
  }
}

void remove_data_out(GtkWidget *widget, gpointer data) {
  Deposit *deposit = (Deposit *)data;
  (void)widget;
  GtkTreeIter iter;
  GtkTreeIter last_iter;
  gint count = gtk_tree_model_iter_n_children(
      GTK_TREE_MODEL(deposit->listbox_output), NULL);
  if (count > 0) {
    gboolean result = gtk_tree_model_get_iter_first(
        GTK_TREE_MODEL(deposit->listbox_output), &iter);
    if (count > 1) {
      if (result) {
        while (gtk_tree_model_iter_next(GTK_TREE_MODEL(deposit->listbox_output),
                                        &iter)) {
          last_iter = iter;
        }
        gtk_list_store_remove(deposit->listbox_output, &last_iter);
      }
    } else {
      gtk_list_store_remove(deposit->listbox_output, &iter);
    }
  }
}

void add_window(GtkWidget *widget, gpointer data) {
  Deposit *deposit = (Deposit *)data;
  (void)widget;
  if (deposit->closed_window_in != NULL) {
    if (GTK_IS_WIDGET(deposit->closed_window_in)) {
      gtk_window_close(GTK_WINDOW(deposit->closed_window_in));
    }
  }

  GtkWidget *window_add = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window_add), "пополнение");
  gtk_window_set_default_size(GTK_WINDOW(window_add), 300, 100);
  gtk_container_set_border_width(GTK_CONTAINER(window_add), 10);
  gtk_window_set_resizable(GTK_WINDOW(window_add), FALSE);

  deposit->closed_window_in = window_add;

  GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_container_add(GTK_CONTAINER(window_add), vbox);

  GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

  GtkWidget *grid_1 = gtk_grid_new();
  gtk_grid_set_row_homogeneous(GTK_GRID(grid_1), TRUE);
  gtk_grid_set_row_spacing(GTK_GRID(grid_1), 5);
  gtk_grid_set_column_spacing(GTK_GRID(grid_1), 10);
  gtk_box_pack_start(GTK_BOX(hbox), grid_1, TRUE, TRUE, 0);

  GtkWidget *label = gtk_label_new("Сумма пополнения:");
  gtk_grid_attach(GTK_GRID(grid_1), label, 0, 0, 1, 1);

  GtkWidget *entry = gtk_entry_new();
  gtk_widget_set_size_request(entry, 100, 30);
  gtk_grid_attach(GTK_GRID(grid_1), entry, 1, 0, 1, 1);
  deposit->sum_in = entry;

  GtkWidget *label_1 = gtk_label_new("Месяцы:");
  gtk_grid_attach(GTK_GRID(grid_1), label_1, 0, 1, 1, 1);

  GtkWidget *entry_1 = gtk_entry_new();
  gtk_widget_set_size_request(entry_1, 100, 30);
  gtk_grid_attach(GTK_GRID(grid_1), entry_1, 1, 1, 1, 1);
  deposit->month_in = entry_1;

  GtkWidget *button_end = gtk_button_new_with_label("Добавить");
  gtk_box_pack_start(GTK_BOX(vbox), button_end, FALSE, FALSE, 0);
  g_signal_connect(G_OBJECT(button_end), "clicked", G_CALLBACK(add_data_in),
                   deposit);
  gtk_widget_show_all(window_add);
}

void withdrawal_window(GtkWidget *widget, gpointer data) {
  Deposit *deposit = (Deposit *)data;
  (void)widget;
  if (deposit->closed_window_out != NULL) {
    if (GTK_IS_WIDGET(deposit->closed_window_out)) {
      gtk_window_close(GTK_WINDOW(deposit->closed_window_out));
    }
  }
  GtkWidget *withdrawal_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(withdrawal_window), "снятие");
  gtk_window_set_default_size(GTK_WINDOW(withdrawal_window), 300, 100);
  gtk_container_set_border_width(GTK_CONTAINER(withdrawal_window), 10);
  gtk_window_set_resizable(GTK_WINDOW(withdrawal_window), FALSE);

  deposit->closed_window_out = withdrawal_window;

  GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_container_add(GTK_CONTAINER(withdrawal_window), vbox);

  GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 0);

  GtkWidget *grid_1 = gtk_grid_new();
  gtk_grid_set_row_homogeneous(GTK_GRID(grid_1), TRUE);
  gtk_grid_set_row_spacing(GTK_GRID(grid_1), 5);
  gtk_grid_set_column_spacing(GTK_GRID(grid_1), 10);
  gtk_box_pack_start(GTK_BOX(hbox), grid_1, TRUE, TRUE, 0);

  GtkWidget *label = gtk_label_new("Сумма снятия:");
  gtk_grid_attach(GTK_GRID(grid_1), label, 0, 0, 1, 1);

  GtkWidget *entry2 = gtk_entry_new();
  gtk_widget_set_size_request(entry2, 100, 30);
  gtk_grid_attach(GTK_GRID(grid_1), entry2, 1, 0, 1, 1);
  deposit->sum_out = entry2;

  GtkWidget *label_1 = gtk_label_new("Месяцы:");
  gtk_grid_attach(GTK_GRID(grid_1), label_1, 0, 1, 1, 1);

  GtkWidget *entry3 = gtk_entry_new();
  gtk_widget_set_size_request(entry3, 100, 30);
  gtk_grid_attach(GTK_GRID(grid_1), entry3, 1, 1, 1, 1);
  deposit->month_out = entry3;

  GtkWidget *button_end = gtk_button_new_with_label("Добавить");
  gtk_box_pack_start(GTK_BOX(vbox), button_end, FALSE, FALSE, 0);
  g_signal_connect(G_OBJECT(button_end), "clicked", G_CALLBACK(add_data_out),
                   deposit);

  gtk_widget_show_all(withdrawal_window);
}

void deposit_calc(GtkWidget *widget, gpointer data) {
  Deposit *deposit = (Deposit *)data;
  (void)widget;
  double accrued_interest;
  double tax_amount;
  double amount_on_deposit;
  int replenishment_count = 0;
  double replenishment[1000][2];
  int withdraw_count = 0;
  double withdraw[1000][2];
  double deposit_amount = (double)gtk_spin_button_get_value(
      GTK_SPIN_BUTTON(deposit->deposit_amount));
  int month_term =
      (int)gtk_spin_button_get_value(GTK_SPIN_BUTTON(deposit->month_term));
  double interest_rate = (double)gtk_spin_button_get_value(
      GTK_SPIN_BUTTON(deposit->interest_rate));
  double tax_rate =
      (double)gtk_spin_button_get_value(GTK_SPIN_BUTTON(deposit->tax_rate));
  int repayment_periodicity = (double)gtk_spin_button_get_value(
      GTK_SPIN_BUTTON(deposit->repayment_periodicity));

  GtkTreeIter iter;
  int i = 0;
  gboolean valid = gtk_tree_model_get_iter_first(
      GTK_TREE_MODEL(deposit->listbox_input), &iter);
  while (valid) {
    gint month;
    gdouble amount;
    gtk_tree_model_get(GTK_TREE_MODEL(deposit->listbox_input), &iter, 0, &month,
                       1, &amount, -1);
    replenishment[i][1] = (double)month;
    replenishment[i][0] = amount;
    i++;
    valid =
        gtk_tree_model_iter_next(GTK_TREE_MODEL(deposit->listbox_input), &iter);
  }
  replenishment_count = i;

  int j = 0;
  gboolean valid_1 = gtk_tree_model_get_iter_first(
      GTK_TREE_MODEL(deposit->listbox_output), &iter);
  while (valid_1) {
    gint month_1;
    gdouble amount_1;
    gtk_tree_model_get(GTK_TREE_MODEL(deposit->listbox_output), &iter, 0,
                       &month_1, 1, &amount_1, -1);
    withdraw[j][1] = (double)month_1;
    withdraw[j][0] = amount_1;
    j++;
    valid_1 = gtk_tree_model_iter_next(GTK_TREE_MODEL(deposit->listbox_output),
                                       &iter);
  }
  withdraw_count = j;

  int rc = calc_deposit(deposit_amount, month_term, interest_rate, tax_rate,
                        repayment_periodicity, deposit->flag, replenishment,
                        replenishment_count, withdraw, withdraw_count,
                        &accrued_interest, &tax_amount, &amount_on_deposit);
  if (!rc) {
    gchar *accrued_interest_str =
        g_strdup_printf("Начисленные проценты:\n%.2lf", accrued_interest);
    gchar *tax_amount_str = g_strdup_printf("Сумма налога:\n%.2lf", tax_amount);
    gchar *amount_on_deposit_str =
        g_strdup_printf("Сумма на депозите:\n%.2lf", amount_on_deposit);

    gtk_label_set_text(GTK_LABEL(deposit->accrued_interest),
                       accrued_interest_str);
    gtk_label_set_text(GTK_LABEL(deposit->tax_amount), tax_amount_str);
    gtk_label_set_text(GTK_LABEL(deposit->amount_on_deposit),
                       amount_on_deposit_str);

    g_free(accrued_interest_str);
    g_free(tax_amount_str);
    g_free(amount_on_deposit_str);
  }
}

void depos_destroy(GtkWidget *widget, gpointer data) {
  Deposit *deposit = (Deposit *)data;
  if (GTK_IS_WIDGET(deposit->closed_window_out)) {
    gtk_window_close(GTK_WINDOW(deposit->closed_window_out));
  }
  if (GTK_IS_WIDGET(deposit->closed_window_in)) {
    gtk_window_close(GTK_WINDOW(deposit->closed_window_in));
  }
}

static void button_deposit(GtkWidget *widget) {
  Deposit *deposit = g_malloc(sizeof(Deposit));
  (void)widget;
  deposit->flag = 0;
  deposit->closed_window_in = NULL;
  deposit->closed_window_out = NULL;
  GtkWidget *window_deposit = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window_deposit), "Депозит");
  gtk_window_set_default_size(GTK_WINDOW(window_deposit), 700, 200);
  gtk_container_set_border_width(GTK_CONTAINER(window_deposit), 10);
  gtk_window_set_resizable(GTK_WINDOW(window_deposit), FALSE);

  GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_container_add(GTK_CONTAINER(window_deposit), vbox);

  GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);

  GtkWidget *grid_1 = gtk_grid_new();
  gtk_grid_set_row_homogeneous(GTK_GRID(grid_1), TRUE);
  gtk_grid_set_row_spacing(GTK_GRID(grid_1), 5);
  gtk_grid_set_column_spacing(GTK_GRID(grid_1), 10);
  gtk_box_pack_start(GTK_BOX(hbox), grid_1, TRUE, TRUE, 0);

  GtkWidget *label = gtk_label_new("Сумма депозита");
  gtk_grid_attach(GTK_GRID(grid_1), label, 0, 0, 1, 1);
  GtkWidget *deposit_amount = gtk_spin_button_new_with_range(0, 10000000, 1);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(deposit_amount), 1000000);
  gtk_grid_attach(GTK_GRID(grid_1), deposit_amount, 1, 0, 1, 1);
  deposit->deposit_amount = deposit_amount;

  GtkWidget *label_1 = gtk_label_new("Срок размещения");
  gtk_grid_attach(GTK_GRID(grid_1), label_1, 0, 1, 1, 1);
  GtkWidget *month_term = gtk_spin_button_new_with_range(1, 100000, 1);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(month_term), 12);
  gtk_grid_attach(GTK_GRID(grid_1), month_term, 1, 1, 1, 1);
  deposit->month_term = month_term;

  GtkWidget *label_2 = gtk_label_new("Процентная ставка");
  gtk_grid_attach(GTK_GRID(grid_1), label_2, 0, 2, 1, 1);
  GtkWidget *interest_rate = gtk_spin_button_new_with_range(0.5, 100, 0.5);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(interest_rate), 5);
  gtk_grid_attach(GTK_GRID(grid_1), interest_rate, 1, 2, 1, 1);
  deposit->interest_rate = interest_rate;

  GtkWidget *label_3 = gtk_label_new("Налоговая ставка");
  gtk_grid_attach(GTK_GRID(grid_1), label_3, 0, 3, 1, 1);
  GtkWidget *tax_rate = gtk_spin_button_new_with_range(0, 100, 0.5);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(tax_rate), 2);
  gtk_grid_attach(GTK_GRID(grid_1), tax_rate, 1, 3, 1, 1);
  deposit->tax_rate = tax_rate;

  GtkWidget *label_4 = gtk_label_new("Периодичность выплат");
  gtk_grid_attach(GTK_GRID(grid_1), label_4, 0, 4, 1, 1);
  GtkWidget *repayment_periodicity = gtk_spin_button_new_with_range(1, 6, 1);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(repayment_periodicity), 1);
  gtk_grid_attach(GTK_GRID(grid_1), repayment_periodicity, 1, 4, 1, 1);
  deposit->repayment_periodicity = repayment_periodicity;

  GtkWidget *check_button = gtk_check_button_new_with_label("Капитализация");
  g_signal_connect(check_button, "toggled", G_CALLBACK(on_checkbox_toggled),
                   deposit);
  gtk_box_pack_start(GTK_BOX(vbox), check_button, FALSE, FALSE, 0);

  GtkWidget *list;

  deposit->listbox_input = gtk_list_store_new(2, G_TYPE_INT, G_TYPE_DOUBLE);

  list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(deposit->listbox_input));
  g_object_unref(deposit->listbox_input);
  deposit->treeview = GTK_TREE_VIEW(list);
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("Месяц", renderer, "text",
                                                    0, NULL);
  gtk_tree_view_append_column(deposit->treeview, column);
  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("Сумма", renderer, "text",
                                                    1, NULL);
  gtk_tree_view_append_column(deposit->treeview, column);

  GtkWidget *scrolled_window_1 = gtk_scrolled_window_new(NULL, NULL);
  gtk_widget_set_size_request(scrolled_window_1, 300, 100);
  gtk_widget_set_tooltip_text(scrolled_window_1, "Пополнение");
  gtk_container_add(GTK_CONTAINER(scrolled_window_1), list);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window_1),
                                 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_grid_attach(GTK_GRID(grid_1), scrolled_window_1, 3, 0, 1, 2);

  GtkWidget *button_plus_in = gtk_button_new_with_label("+");
  gtk_grid_attach(GTK_GRID(grid_1), button_plus_in, 4, 0, 1, 1);
  g_signal_connect(G_OBJECT(button_plus_in), "clicked", G_CALLBACK(add_window),
                   deposit);

  GtkWidget *button_minus_in = gtk_button_new_with_label("-");
  gtk_grid_attach(GTK_GRID(grid_1), button_minus_in, 4, 1, 1, 1);
  g_signal_connect(G_OBJECT(button_minus_in), "clicked",
                   G_CALLBACK(remove_data_in), deposit);

  GtkWidget *list_1;
  deposit->listbox_output = gtk_list_store_new(2, G_TYPE_INT, G_TYPE_DOUBLE);

  list_1 =
      gtk_tree_view_new_with_model(GTK_TREE_MODEL(deposit->listbox_output));
  g_object_unref(deposit->listbox_output);
  deposit->treeview_1 = GTK_TREE_VIEW(list_1);
  GtkCellRenderer *renderer_1;
  GtkTreeViewColumn *column_1;
  renderer_1 = gtk_cell_renderer_text_new();
  column_1 = gtk_tree_view_column_new_with_attributes("Месяц", renderer_1,
                                                      "text", 0, NULL);
  gtk_tree_view_append_column(deposit->treeview_1, column_1);
  renderer_1 = gtk_cell_renderer_text_new();
  column_1 = gtk_tree_view_column_new_with_attributes("Сумма", renderer_1,
                                                      "text", 1, NULL);
  gtk_tree_view_append_column(deposit->treeview_1, column_1);

  GtkWidget *scrolled_window_2 = gtk_scrolled_window_new(NULL, NULL);
  gtk_widget_set_size_request(scrolled_window_2, 300, 100);
  gtk_widget_set_tooltip_text(scrolled_window_2, "Снятие");
  gtk_container_add(GTK_CONTAINER(scrolled_window_2), list_1);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window_2),
                                 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_grid_attach(GTK_GRID(grid_1), scrolled_window_2, 3, 2, 1, 2);

  GtkWidget *button_plus_out = gtk_button_new_with_label("+");
  gtk_grid_attach(GTK_GRID(grid_1), button_plus_out, 4, 2, 1, 1);

  g_signal_connect(G_OBJECT(button_plus_out), "clicked",
                   G_CALLBACK(withdrawal_window), deposit);

  GtkWidget *button_minus_out = gtk_button_new_with_label("-");
  gtk_grid_attach(GTK_GRID(grid_1), button_minus_out, 4, 3, 1, 1);
  g_signal_connect(G_OBJECT(button_minus_out), "clicked",
                   G_CALLBACK(remove_data_out), deposit);

  deposit->accrued_interest = gtk_label_new(NULL);
  gtk_grid_attach(GTK_GRID(grid_1), deposit->accrued_interest, 3, 5, 1, 1);

  deposit->tax_amount = gtk_label_new(NULL);
  gtk_grid_attach(GTK_GRID(grid_1), deposit->tax_amount, 3, 6, 1, 1);

  deposit->amount_on_deposit = gtk_label_new(NULL);
  gtk_grid_attach(GTK_GRID(grid_1), deposit->amount_on_deposit, 3, 7, 1, 1);

  GtkWidget *button_end = gtk_button_new_with_label("Рассчитать");
  gtk_box_pack_start(GTK_BOX(vbox), button_end, FALSE, FALSE, 0);

  g_signal_connect(G_OBJECT(button_end), "clicked", G_CALLBACK(deposit_calc),
                   deposit);
  g_signal_connect(G_OBJECT(window_deposit), "destroy",
                   G_CALLBACK(depos_destroy), deposit);

  gtk_widget_show_all(window_deposit);
}

static void button_graphics(GtkWidget *widget, gpointer data) {
  Grath *grath = (Grath *)data;
  (void)widget;
  if (grath->window_g != NULL) {
    if (GTK_IS_WIDGET(grath->window_g)) {
      gtk_window_close(GTK_WINDOW(grath->window_g));
    }
  }
  GtkWidget *window_grath = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window_grath), "График");
  gtk_window_set_default_size(GTK_WINDOW(window_grath), 600, 700);
  gtk_container_set_border_width(GTK_CONTAINER(window_grath), 10);
  gtk_window_set_resizable(GTK_WINDOW(window_grath), FALSE);
  grath->window_g = window_grath;

  GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_container_add(GTK_CONTAINER(window_grath), vbox);
  GtkWidget *entr = gtk_entry_new();

  gtk_entry_set_max_length(GTK_ENTRY(entr), 0);
  gtk_box_pack_start(GTK_BOX(vbox), entr, FALSE, FALSE, 0);
  grath->entry = entr;
  GtkEntry *entry1 = GTK_ENTRY(grath->entry_main);
  const gchar *entry_text = gtk_entry_get_text(entry1);
  grath->entry_text = entry_text;
  gtk_entry_set_text(GTK_ENTRY(entr), entry_text);

  GtkWidget *area = gtk_drawing_area_new();
  gtk_box_pack_start(GTK_BOX(vbox), area, TRUE, TRUE, 0);
  g_signal_connect(G_OBJECT(area), "draw", G_CALLBACK(draw_callback), grath);
  grath->area = area;
  GtkWidget *grid_1 = gtk_grid_new();
  gtk_grid_set_row_spacing(GTK_GRID(grid_1), 5);
  gtk_grid_set_column_spacing(GTK_GRID(grid_1), 5);
  gtk_box_pack_start(GTK_BOX(vbox), grid_1, FALSE, FALSE, 0);

  GtkWidget *button_zoom_in = gtk_spin_button_new_with_range(-1000, 1000, 1);
  gtk_grid_attach(GTK_GRID(grid_1), button_zoom_in, 0, 0, 1, 1);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(button_zoom_in), -10);
  g_signal_connect(G_OBJECT(button_zoom_in), "value-changed",
                   G_CALLBACK(button_draw_graph), grath);
  GtkWidget *button_zoom_out = gtk_spin_button_new_with_range(-1000, 1000, 1);
  gtk_grid_attach(GTK_GRID(grid_1), button_zoom_out, 1, 0, 1, 1);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(button_zoom_out), 10);
  g_signal_connect(G_OBJECT(button_zoom_out), "value-changed",
                   G_CALLBACK(button_draw_graph), grath);
  grath->button_zoom_in = button_zoom_in;
  grath->button_zoom_out = button_zoom_out;

  gtk_widget_show_all(window_grath);
}

int main(int argc, char *argv[]) {
  gtk_init(&argc, &argv);
  Grath *grath = g_malloc(sizeof(Grath));
  grath->window_g = NULL;
  GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Калькулятор");
  gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
  gtk_container_set_border_width(GTK_CONTAINER(window), 10);
  g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit),
                   NULL);
  gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
  GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  GtkWidget *entry = gtk_entry_new();
  grath->entry_main = entry;
  gtk_entry_set_max_length(GTK_ENTRY(entry), 0);
  gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 0);

  GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
  gtk_box_pack_start(GTK_BOX(vbox), hbox, TRUE, TRUE, 0);

  GtkWidget *grid_1 = gtk_grid_new();
  gtk_grid_set_row_spacing(GTK_GRID(grid_1), 5);
  gtk_grid_set_column_spacing(GTK_GRID(grid_1), 5);
  gtk_box_pack_start(GTK_BOX(hbox), grid_1, TRUE, TRUE, 0);

  GtkWidget *grid_2 = gtk_grid_new();
  gtk_grid_set_row_spacing(GTK_GRID(grid_2), 5);
  gtk_box_pack_start(GTK_BOX(hbox), grid_2, FALSE, FALSE, 0);

  GtkWidget *button_grath = gtk_button_new_with_label("График");
  gtk_grid_attach(GTK_GRID(grid_2), button_grath, 0, 0, 2, 2);
  g_signal_connect(G_OBJECT(button_grath), "clicked",
                   G_CALLBACK(button_graphics), grath);
  GtkWidget *button_credit = gtk_button_new_with_label("Кредит");
  gtk_grid_attach(GTK_GRID(grid_2), button_credit, 0, 2, 2, 2);
  g_signal_connect(G_OBJECT(button_credit), "clicked",
                   G_CALLBACK(button_credits), NULL);
  GtkWidget *button_depos = gtk_button_new_with_label("Депозит");
  gtk_grid_attach(GTK_GRID(grid_2), button_depos, 0, 4, 2, 2);
  g_signal_connect(G_OBJECT(button_depos), "clicked",
                   G_CALLBACK(button_deposit), NULL);

  const gchar *buttons[] = {"sin", "cos", "tan", "x",    "(", ")", "ln",
                            "log", "AC",  "^",   "sqrt", "⌫", "7", "8",
                            "9",   "/",   "4",   "5",    "6", "*", "1",
                            "2",   "3",   "-",   "0",    ",", "=", "+"};

  for (int i = 0; i < 28; i++) {
    GtkWidget *button = gtk_button_new_with_label(buttons[i]);
    gtk_grid_attach(GTK_GRID(grid_1), button, i % 4, i / 4, 1, 1);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(button_clicked),
                     grath);
  }
  const gchar *buttons2[] = {"asin", "acos", "atan"};

  for (int i = 0; i < 3; i++) {
    GtkWidget *button = gtk_button_new_with_label(buttons2[i]);
    gtk_grid_attach(GTK_GRID(grid_1), button, 5, i, 1, 1);
    g_signal_connect(G_OBJECT(button), "clicked", G_CALLBACK(button_clicked),
                     grath);
  }
  GtkWidget *label = gtk_label_new("X:");
  gtk_grid_attach(GTK_GRID(grid_1), label, 0, 7, 1, 2);
  GtkWidget *button_size_x = gtk_spin_button_new_with_range(-100000, 100000, 1);
  gtk_grid_attach(GTK_GRID(grid_1), button_size_x, 1, 7, 2, 2);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(button_size_x), 0);
  grath->x_field = button_size_x;

  gtk_widget_show_all(window);
  gtk_main();

  return 0;
}
