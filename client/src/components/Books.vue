<template>
  <v-container justify-center>
    <span class="display-1">Books</span>
    <v-divider></v-divider>
    <br>
    <v-btn
      color="green"
      @click.stop="dialog = true"
    >Add book</v-btn>
    <br><br>
    <v-dialog
      v-model="dialog"
      max-width="300"
    >
      <v-card>
        <v-card-title class="headline">Add a new book?</v-card-title>
        <v-card-text>
          <v-form ref="addBookModal" >
            <v-text-field
              v-model="addBookForm.title"
              label="Title"
              required
            ></v-text-field>
            <v-text-field
              v-model="addBookForm.author"
              label="Author"
              required
            ></v-text-field>
            <v-checkbox
              v-model="addBookForm.read"
              label="Read?"
              required
            ></v-checkbox>
            <v-btn @click="onSubmit" color="blue">Submit</v-btn>
            <v-btn @click="onReset" color="red">Reset</v-btn>
          </v-form>
        </v-card-text>
      </v-card>
    </v-dialog>
    <v-data-table
      :headers="headers"
      :items="books"
      class="elevation-1"
      hide-actions
    >
      <template v-slot:items="props">
        <td>{{ props.item.title }}</td>
        <td>{{ props.item.author }}</td>
        <td>
          <template v-if="props.item.read">
            Yes
          </template>
          <template v-else>
            No
          </template>
        <td>
          <v-btn color="orange ">Update</v-btn>
          <v-btn color="red">Delete</v-btn>
        </td>
      </template>
    </v-data-table>
  </v-container>
</template>

<script>
import axios from 'axios'

export default {
  name: 'Book',
  data () {
    return {
      headers: [
        {
          text: 'Title',
          sortable: false
        },
        { text: 'Author' },
        { text: 'Read?' },
        { text: '' }
      ],
      books: [],
      dialog: false,
      addBookForm: {
        title: '',
        author: '',
        read: []
      },
      isRead: false
    }
  },
  methods: {
    getBooks () {
      const path = 'http://localhost:5000/books'
      axios.get(path)
        .then((res) => {
          this.books = res.data.books
        })
        .catch((error) => {
          // eslint-disable-next-line
          console.error(error);
        })
    },
    addBook (payload) {
      const path = 'http://localhost:5000/books'
      axios.post(path, payload)
        .then(() => {
          this.getBooks()
        })
        .catch((error) => {
          console.log(error)
        })
    },
    initForm () {
      this.addBookForm.title = ''
      this.addBookForm.author = ''
      this.addBookForm.read = ''
    },
    onSubmit () {
      this.dialog = false
      let read = false
      if (this.addBookForm.read[0]) read = true
      const payload = {
        title: this.addBookForm.title,
        author: this.addBookForm.author,
        read // property shorthand
      }
      this.addBook(payload)
      this.initForm()
    },
    onReset () {
      this.dialog = false
      this.initForm()
    }
  },
  created () {
    this.getBooks()
  }
}
</script>
