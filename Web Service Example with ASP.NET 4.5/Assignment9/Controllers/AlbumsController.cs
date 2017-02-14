using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;

namespace Assignment9.Controllers
{
    public class AlbumsController : Controller
    {
        Manager m = new Manager();

        // GET: Albums
        [Authorize]
        public ActionResult Index()
        {
            var o = m.AlbumGetAll();
            return View(o);
        }

        // GET: Albums/Details/5
        [Authorize]
        public ActionResult Details(int? id)
        {
            var o = m.AlbumGetOne(id.GetValueOrDefault());

            if ( o == null )
            {
                return HttpNotFound();
            }
            else
            {
                return View(o);
            }          
        }

        // GET: Albums/Create
        [Authorize(Roles ="Coordinator")]
        public ActionResult Create()
        {
            var form = new AlbumAddForm();

            // set default datetime
           // form.ReleaseDate = DateTime.Now;

            // create list
            form.GenreList = new SelectList(m.GenreGetAll(), "Id", "Name");

            return View(form);
        }

        // POST: Albums/Create
        [HttpPost,Authorize(Roles = "Coordinator")]
        public ActionResult Create(FormCollection collection)
        {
            try
            {
                // TODO: Add insert logic here

                return RedirectToAction("Index");
            }
            catch
            {
                return View();
            }
        }

        //GET: Albums/{id}/AddTrack
        [Authorize(Roles = "Clerk")]
        public ActionResult AddTrack(int? id)
        {
            // grab reference to this album
            var o = m.AlbumGetOne(id.GetValueOrDefault());

            if ( o == null )
            {
                return HttpNotFound();
            }

            var form = new TrackAddForm();
            
            // Copy album details into track
            form.AlbumId = o.Id;
            form.AlbumName = o.Name;

            // update genre list
            form.GenreList = new SelectList(m.GenreGetAll(), "Id", "Name");

            return View(form);
        }

        //POST: Albums/AddTrack
        [HttpPost, Authorize(Roles ="Clerk")]
        public ActionResult AddTrack(TrackAdd newItem)
        {
            if (!ModelState.IsValid)
            {
                var errors = ModelState.SelectMany(x => x.Value.Errors.Select(z => z.Exception));
                return View(newItem);
            }
            var ItemToAdd = m.AddTrack(newItem);
            if ( ItemToAdd == null )
            {
                return View("index");
            }
            else
            {
                return RedirectToAction("Details", "Tracks", new { id = ItemToAdd.Id });
            }
        }
        // GET: Albums/Edit/5
        public ActionResult Edit(int id)
        {
            return View();
        }

        // POST: Albums/Edit/5
        [HttpPost]
        public ActionResult Edit(int id, FormCollection collection)
        {
            try
            {
                // TODO: Add update logic here

                return RedirectToAction("Index");
            }
            catch
            {
                return View();
            }
        }

        // GET: Albums/Delete/5
        [Authorize(Roles = "Coordinator")]
        public ActionResult Delete(int? id)
        {
            var o = m.AlbumGetOne(id.GetValueOrDefault());

            if ( o == null )
            {
                return HttpNotFound();
            }
            else
            {
                return View(o);
            }
           
        }

        // POST: Albums/Delete/5
        [HttpPost, Authorize(Roles = "Coordinator")]
        public ActionResult Delete(int? id, AlbumBase Item)
        {
            var ItemToDelete = m.DeleteAlbum(id.GetValueOrDefault());

            return RedirectToAction("index");
        }
    }
}
